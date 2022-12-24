#include "SaveEditArea.h"
#include <functional>

SaveEditArea::SaveEditArea(APIClient &client) : NetEditTab(client, RENDER)
{
}

/** PUBLIC **/

void SaveEditArea::loadAPI()
{
    if (_config == nullptr) {
        throw std::runtime_error("SaveEditArea::load null project config.");
    }
    this->setLoadingState(true);
    for (OCRPage const &page: _pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToRender(page, _config->renderConf, success, error); // TODO : to improve
    }
}

/** PRIVATE **/

