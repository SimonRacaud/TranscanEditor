#include "SaveEditArea.h"
#include <functional>

SaveEditArea::SaveEditArea(APIClient &client) : NetEditTab(client, RENDER)
{
}

/** PUBLIC **/

void SaveEditArea::load(std::vector<OCRPage> const &pages)
{
    this->setPages(pages);
    // API call to render each page
    if (_config == nullptr) {
        throw std::runtime_error("SaveEditArea::load null project config.");
    }
    this->setLoadingState(true);
    for (OCRPage const &page: pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToRender(page, _config->renderConf, success, error); // TODO : to improve
    }
}

void SaveEditArea::loadPage(OCRPage const &page)
{
    this->updatePage(page);
    this->setLoadingState(false);
}

/** PRIVATE **/

