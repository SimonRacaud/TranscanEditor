#include "EditorEditArea.h"
#include <functional>

using namespace std;

EditorEditArea::EditorEditArea(APIClient &client)
    : ATextEditArea(client, CLEAN, RectMode::EDIT_TRAN, false)
{
    connect(this, &EditorEditArea::sigRectFocusChanged, this, &EditorEditArea::onRectFocusChange);
}

void EditorEditArea::loadAPI()
{
    ATextEditArea::loadAPI();
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

/** SLOTS **/

void EditorEditArea::updateAllClusterStyle(RenderConfig const &style)
{
    QList<QGraphicsItem *> items = this->_scene->items();

    for (QGraphicsItem *item : items) {
        EditAreaRect *rect = dynamic_cast<EditAreaRect *>(item);
        if (rect) {
            rect->setStyle(style);
        }
    }
}

void EditorEditArea::updateSelectedClusterStyle(RenderConfig const &style)
{
    try {
        EditAreaRect *rect = (!_selectedItemId.isNull()) ? &this->getRectFromId(_selectedItemId)
                                                         : nullptr;
        if (rect) {
            rect->setStyle(style);
        }
    } catch (std::exception const &err) {
        std::cerr << "EditorEditArea::updateSelectedClusterStyle : Fail. " << err.what() << std::endl;
    }
}

/** PRIVATE **/

void EditorEditArea::onRectFocusChange(EditAreaRect *rect)
{
    if (rect) {
        emit sigFocusEditRect(rect->getData().style);
    } else {
        emit sigUnfocusEditRect();
    }
}
