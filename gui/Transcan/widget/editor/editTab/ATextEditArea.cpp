#include "ATextEditArea.h"

#include <QScrollBar>

using namespace std;

ATextEditArea::ATextEditArea(APIClient &client, ImageMode modeImg, RectMode mode, bool autoReload)
    : NetEditTab(client, modeImg, autoReload), _mode(mode)
{
    connect(_view, &PageGraphicsView::onDoubleClick, this, &ATextEditArea::doubleClickEvent);
}

/** PUBLIC **/

void ATextEditArea::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Display page images
    this->setPagesEditAreas(pages);
}

void ATextEditArea::createAreaRectAtCoord(QPointF const &coord)
{
    const QPoint size(DEF_EDITAREA_SIZE);
    const QPoint origin(coord.x() - (size.x() / 2), coord.y() - (size.y() / 2));

    // Find page under coord coordinates
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    for (QGraphicsItem *pageItem : pageItems) {
        if (pageItem->sceneBoundingRect().contains(coord)) {
            const qreal pagePosY = pageItem->scenePos().y();

            const BlockCluster defaultData = {
                .blocks = {},
                .sentence = "Text",
                .cleanBox = true,
                .polygon = QPolygon(QRect(origin, origin + size).translated(0, -pagePosY)),
                .translation = "",
                .style = this->_config->renderConf,
            };
            this->createAreaRect(defaultData, pagePosY);
            break;
        }
    }
}

void ATextEditArea::removeRect()
{
    if (_selectedItemId.isNull())
        return; // No rect selected
    try {
        EditAreaRect &selectedItem = this->getRectFromId(_selectedItemId);
        QList<QGraphicsItem *> items = this->_scene->items();
        int index = items.indexOf(selectedItem.topLevelItem());
        disconnect(&selectedItem, &EditAreaRect::focusChanged, this, &ATextEditArea::changeFocus);
        if (index == -1) { // not found
            std::cerr << "ATextEditArea::removeRect Unable to find element." << std::endl;
            return; // Abort
        }
        this->_scene->removeItem(&selectedItem);
        delete &selectedItem;
        this->changeFocus(false, nullptr); // Remove focus
    } catch (std::exception const &err) {
        std::cerr << "ATextEditArea::removeRect : fail to remove element. " << err.what() << std::endl;
    }
}

vector<BlockCluster> ATextEditArea::getClusters() const
{
    vector<BlockCluster> result;
//    QList<QGraphicsItem *> items = this->_scene->items();

//    for ( QGraphicsItem *item : items) {
//        EditAreaRect *rect = qgraphicsitem_cast<EditAreaRect *>(item);
//        if (rect) {
//            result.push_back(rect->getData());
//        }
//    }
    return result;
}

OCRPage ATextEditArea::getPage(size_t index)
{
    if (index >= (size_t)_pages.size()) {
        throw std::invalid_argument("ATextEditArea::getPage, invalid index");
    }
    OCRPage page = ImageViewer::getPage(index);

    page.clusters.clear();
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    if ((size_t)pageItems.size() <= index) {
        throw std::runtime_error("ATextEditArea::getPage, invalid _pageGroup size");
    }
    QGraphicsItem *pageItem = pageItems[index]; // Page image widget
    QRectF pageRect = pageItem->sceneBoundingRect();
    for (QGraphicsItem *item : _scene->items()) {
        EditAreaRect *rect = dynamic_cast<EditAreaRect *>(item);
        if (!rect)
            continue; // Not a EditAreaRect
        if (rect->isOnArea(pageRect)) {
            page.clusters.push_back(rect->getData());
        }
    }
    return page;
}

void ATextEditArea::clearRects()
{
    this->_scene->clear();
}

void ATextEditArea::loadAPI()
{
    // Unfocus current rect
    this->changeFocus(false, nullptr);
}

/** PROTECTED **/

void ATextEditArea::keyPressEvent(QKeyEvent *event)
{
    ImageViewer::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete) {
        // remove currently selected EditAreaRect
        this->removeRect();
    }
}

void ATextEditArea::mousePressEvent(QMouseEvent *)
{
    if (!_selectedItemId.isNull() && _focusedRectLostFocus == true) {
            _focusedRectLostFocus = false;
            this->changeFocus(false, nullptr);
    }
}

void ATextEditArea::doubleClickEvent(QMouseEvent *event)
{
    if (!_selectedItemId.isNull()) {
        return; // Editing existing rect. Not creating one.
    }
    const QPointF position = event->position(); // Position on the widget
    QPointF scenePos = this->_view->mapToScene(position.toPoint()); // Position on the scene

    this->createAreaRectAtCoord(scenePos);
}

EditAreaRect &ATextEditArea::getRectFromId(QUuid const &id)
{
    QList<QGraphicsItem *> list = this->_scene->items();

    for (QGraphicsItem *item : list) {
        EditAreaRect *rect = dynamic_cast<EditAreaRect *>(item);
        if (rect && rect->getUuid() == id) {
            return *rect;
        }
    }
    throw std::invalid_argument("ATextEditArea::getRectFromId : "+id.toString().toStdString()+" not found.");
}

void ATextEditArea::createAreaRect(BlockCluster const &data, int pagePosY)
{
    auto *rect = new EditAreaRect(data, _mode, pagePosY);

    this->_scene->addItem(rect);
    connect(rect, &EditAreaRect::focusChanged, this, &ATextEditArea::changeFocus);
}

void ATextEditArea::setPagesEditAreas(vector<OCRPage> const &pages)
{
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    for (OCRPage const &page : pages) {
        for (BlockCluster const &cluster : page.clusters) {
            if (page.index >= pageItems.size()) {
                throw std::invalid_argument("ATextEditArea::setPagesEditAreas, corrupted page index.");
            }
            QGraphicsItem *pageItem = pageItems[page.index]; // Page image widget

            // convert coord from CORE: relative to page => relative to scene
            BlockCluster c = cluster;
            qreal pagePosY = pageItem->scenePos().y();
            this->createAreaRect(c, pagePosY);
        }
    }
}

/** SLOTS **/

void ATextEditArea::changeFocus(bool focused, EditAreaRect *rect)
{
    try {
        EditAreaRect *selectedItem = (!_selectedItemId.isNull()) ? &this->getRectFromId(_selectedItemId) : nullptr;

        if (selectedItem && selectedItem != rect) {
           selectedItem->removeFocus();
        }
        if (focused && rect) { // Focus
           _selectedItemId = rect->getUuid();
           selectedItem = (!_selectedItemId.isNull()) ? &this->getRectFromId(_selectedItemId) : nullptr;
           _focusedRectLostFocus = false;
        } else if (selectedItem == rect) { // Unfocus
            _focusedRectLostFocus = true;
            return; // Don't emit signal
        } else if (rect == nullptr) { // Apply unfocus
            _selectedItemId = QUuid(); // Reset to null
            selectedItem = nullptr;
        }
        emit this->sigRectFocusChanged(selectedItem);
    } catch (std::exception const &err) {
        std::cerr << "ATextEditArea::changeFocus : fail to change focus. " << err.what() << std::endl;
    }
}
