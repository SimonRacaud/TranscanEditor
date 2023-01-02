#include "NetEditTab.h"

NetEditTab::NetEditTab(APIClient &client, ImageMode mode, bool autoReload)
    : ImageViewer(mode), _api(client), _autoReload(autoReload)
{
}

NetEditTab::~NetEditTab()
{
}

void NetEditTab::netError(QString const &message)
{
    std::cerr << "Network error : " << message.toStdString() << std::endl;
    this->setLoadingState(false);
    // TODO : show GUI popup
}

void NetEditTab::setConfig(shared_ptr<ProjectConfig> config)
{
    this->_config = config;
}

void NetEditTab::unload()
{
    // Don't process pending requests' result
    this->_api.abortRequests();
}

void NetEditTab::load(std::vector<OCRPage> const &pages)
{
    try {
        this->setPages(pages);
    } catch (std::exception const& err) {
        std::cerr << "Fail to load pages. " << err.what() << std::endl;
        // TODO: better error management
    }
    if (_autoReload) {
        // API call to render each page
        try {
            this->loadAPI();
        } catch (std::exception const &err) {
            std::cerr << "Fail to send API request. " << err.what() << std::endl;
            // TODO: better error management
        }
    }
}

void NetEditTab::loadPage(OCRPage const &page)
{
    try {
        // Reload scene and update page
        this->updatePage(page);
    } catch (std::exception const &err) {
        std::cerr << "Fail to update the page " << page.index << ". " << err.what() << std::endl;
    }
    this->setLoadingState(false);
    if (!this->_api.pendingReply()) { // All requests completed
        emit this->allAPIRequestsCompleted();
    }
}

void NetEditTab::setLoadingState(bool enable)
{
    // Only hide the loading icon if every requests are complete
    if (enable || !_api.pendingReply()) {
        ImageViewer::setLoadingState(enable);
    }
}
