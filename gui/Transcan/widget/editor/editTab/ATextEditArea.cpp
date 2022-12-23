#include "ATextEditArea.h"

#include <QScrollBar>

using namespace std;

ATextEditArea::ATextEditArea(APIClient &client, ImageMode modeImg, RectMode mode) : NetEditTab(client, modeImg), _mode(mode)
{
    connect(_view, &PageGraphicsView::onDoubleClick, this, &ATextEditArea::doubleClickEvent);
}

/** PUBLIC **/

void ATextEditArea::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Display page images
    this->setPagesEditAreas(pages);
}

void ATextEditArea::createAreaRectAtCoord(QPoint const &coord)
{
    const QPoint size(DEF_EDITAREA_SIZE);
    const QPoint origin(coord.x() - (size.x() / 2), coord.y() - (size.y() / 2));

    const BlockCluster defaultData = {
        .blocks = {},
        .sentence = "Text",
        .cleanBox = true,
        .polygon = QPolygon(QRect(origin, origin + size)),
        .translation = "",
        .font = QFont(),
        .color = Qt::black,
        .lineHeight = DEF_LINE_HEIGHT,
        .strokeWidth = 8
    };
    // Find page under coord coordinates
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    auto it = std::find_if(pageItems.begin(), pageItems.end(),
                           [coord](QGraphicsItem *item){ return item->contains(QPointF(coord)); });
    if (it == pageItems.end()) {
        std::invalid_argument("ATextEditArea::createAreaRectAtCoord Coordinate outside pages range.");
    }
    qreal pagePosY = (*it)->scenePos().y();
    this->createAreaRect(defaultData, pagePosY);
}

void ATextEditArea::removeRect()
{
    if (_focusedRect) {
        QList<QGraphicsItem *> items = this->_scene->items();
        int index = items.indexOf(_focusedRect->topLevelItem());
        disconnect(_focusedRect, &EditAreaRect::focusChanged, this, &ATextEditArea::changeFocus);
        if (index == -1) { // not found
            _focusedRect = nullptr;
            std::cerr << "ATextEditArea::removeRect Unable to find element." << std::endl;
            return; // Abort
        }
        this->_scene->removeItem(_focusedRect);
        delete _focusedRect;
        _focusedRect = nullptr;
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
    if (pageItems.size() <= index) {
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

/** PROTECTED **/

void ATextEditArea::keyPressEvent(QKeyEvent *event)
{
    ImageViewer::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete) {
        // remove currently selected EditAreaRect
        this->removeRect();
    }
}

void ATextEditArea::doubleClickEvent(QMouseEvent *event)
{
    const QPointF position = event->position(); // Position on the widget
    QPointF scenePos = this->_view->mapToScene(position.toPoint()); // Position on the scene

    this->createAreaRectAtCoord(scenePos.toPoint());
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

void ATextEditArea::changeFocus(bool focused, EditAreaRect &rect)
{
    if (focused) {
        this->_focusedRect = &rect;
    } else if (_focusedRect == &rect) {
        this->_focusedRect = nullptr;
    }
}
