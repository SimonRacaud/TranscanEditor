#include "EditorEditArea.h"
#include <functional>

using namespace std;

EditorEditArea::EditorEditArea(APIClient &client): ATextEditArea(client, CLEAN, RectMode::EDIT_TRAN)
{
}

void EditorEditArea::load(std::vector<OCRPage> const &pages)
{
    // Load pages
    this->setPages(pages);
    // Translation API Call (if first load)
    if (this->_config == nullptr) {
        throw std::runtime_error("EditorEditArea::load null project config.");
    }
    this->setLoadingState(true);
    for (OCRPage const &page: pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToTranslate(page, _config->transService, success, error);
    }
}

void EditorEditArea::loadPage(OCRPage const &page)
{
    try {
        this->setLoadingState(false);
        this->updatePage(page);
    } catch (std::exception const &err) {
        std::cerr << "EditorEditArea::loadPage error: " << err.what() << std::endl;
    }
}
