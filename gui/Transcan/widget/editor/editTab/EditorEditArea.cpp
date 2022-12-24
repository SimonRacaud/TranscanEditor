#include "EditorEditArea.h"
#include <functional>

using namespace std;

EditorEditArea::EditorEditArea(APIClient &client)
    : ATextEditArea(client, CLEAN, RectMode::EDIT_TRAN, false)
{
}

void EditorEditArea::loadAPI()
{
    if (this->_config == nullptr) {
        throw std::runtime_error("EditorEditArea::load null project config.");
    }
    this->setLoadingState(true);
    for (OCRPage const &page: _pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToTranslate(page, _config->transService, success, error);
    }
}
