#include "NetEditTab.h"

NetEditTab::NetEditTab(APIClient &client, ImageMode mode) : ImageViewer(mode), _api(client)
{
}

void NetEditTab::netError(QString const &message)
{
    std::cerr << "Network error : " << message.toStdString() << std::endl;
    // TODO : show GUI popup
}

void NetEditTab::setConfig(ProjectConfig const &config)
{
    this->_config = &config;
}
