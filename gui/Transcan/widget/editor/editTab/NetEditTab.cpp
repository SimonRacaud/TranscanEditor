#include "NetEditTab.h"

NetEditTab::NetEditTab(APIClient &client, ImageMode mode) : ImageViewer(mode), _api(client)
{
}

NetEditTab::~NetEditTab()
{
    delete _config;
}

void NetEditTab::netError(QString const &message)
{
    std::cerr << "Network error : " << message.toStdString() << std::endl;
    this->setLoadingState(false);
    // TODO : show GUI popup
}

void NetEditTab::setConfig(ProjectConfig const &config)
{
    this->_config = new ProjectConfig(config);
}

void NetEditTab::unload()
{
    // Don't process pending requests' result
    this->_api.abortRequests();
}
