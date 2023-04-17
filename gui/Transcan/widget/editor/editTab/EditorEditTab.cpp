#include "EditorEditTab.h"
#include <functional>

using namespace std;

EditorEditTab::EditorEditTab(APIClient &client)
    : ATextEditTab(client, CLEAN, RectMode::EDIT_TRAN, false)
{
    connect(this, &EditorEditTab::sigRectFocusChanged, this, &EditorEditTab::onRectFocusChange);
}

void EditorEditTab::loadAPI()
{
    ATextEditTab::loadAPI();
    if (this->_config == nullptr) {
        throw std::runtime_error("EditorEditTab::load null project config.");
    }
    this->setLoadingState(true);
    for (OCRPage const &page: _pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToTranslate(page, _config->transService, success, error);
    }
}

void EditorEditTab::unload()
{
    ATextEditTab::unload();

    this->renderSceneToFiles();
}

/** SLOTS **/

void EditorEditTab::updateAllClusterStyle(RenderConfig const &style)
{
    QList<QGraphicsItem *> items = this->_scene->items();

    for (QGraphicsItem *item : items) {
        TextEditBox *rect = dynamic_cast<TextEditBox *>(item);
        if (rect) {
            rect->setStyle(style);
        }
    }
    this->_config->renderConf = style;
}

void EditorEditTab::updateSelectedClusterStyle(RenderConfig const &style)
{
    try {
        TextEditBox *rect = (!_selectedItemId.isNull()) ? &this->getRectFromId(_selectedItemId)
                                                         : nullptr;
        if (rect) {
            rect->setStyle(style);
        }
    } catch (std::exception const &err) {
        std::cerr << "EditorEditTab::updateSelectedClusterStyle : Fail. " << err.what() << std::endl;
    }
}

/** PRIVATE **/

void EditorEditTab::onRectFocusChange(TextEditBox *rect)
{
    if (rect) {
        emit sigFocusEditRect(rect->getData().style);
    } else {
        emit sigUnfocusEditRect();
    }
}

void EditorEditTab::renderSceneToFiles()
{
    this->enableEditBoxesRect(false); // Hide boxes borders
    _scene->clearSelection();                                                  // Selections would also render to the file
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    size_t idx = 0;
    for (QGraphicsItem const *item : pageItems) {
        QImage image(item->boundingRect().size().toSize(), QImage::Format_ARGB32);  // Create the image with the exact size of the shrunk scene
        image.fill(Qt::transparent);                                              // Start all pixels transparent
        QPainter painter(&image);
        _scene->setSceneRect(item->sceneBoundingRect());
        _scene->render(&painter);
        const QString &filename = QString::fromStdString("render_page_" + std::to_string(idx + 1) + ".png");
        this->_pages[idx].renderImagePath = this->_config->destPath + "/" + filename;
        image.save(this->_pages[idx].renderImagePath);
        qDebug() << "(info) Save page " << idx << " to " << this->_pages[idx].renderImagePath;
        idx++;
    }
    _scene->setSceneRect(_scene->itemsBoundingRect());                          // Re-shrink the scene to it's bounding contents
}

void EditorEditTab::enableEditBoxesRect(bool enable)
{
    this->foreachTextEditBox([enable](TextEditBox &rect) {
        rect.enableBoxView(enable);
    });
}
