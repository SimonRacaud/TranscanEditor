#include "ATextEditTab.h"

#include <QScrollBar>

using namespace std;

ATextEditTab::ATextEditTab(APIClient &client, ImageMode modeImg, RectMode mode, bool autoReload)
    : NetEditTab(client, modeImg, autoReload), _mode(mode)
{
    connect(_view, &PageGraphicsView::onDoubleClick, this, &ATextEditTab::doubleClickEvent);
}

/** PUBLIC **/

void ATextEditTab::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Display page images
    this->setPagesEditAreas(pages);
}

void ATextEditTab::createAreaRectAtCoord(QPointF const &coord)
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

void ATextEditTab::removeRect()
{
    if (_selectedItemId.isNull())
        return; // No rect selected
    try {
        TextEditBox &selectedItem = this->getRectFromId(_selectedItemId);
        QList<QGraphicsItem *> items = this->_scene->items();
        int index = items.indexOf(selectedItem.topLevelItem());
        disconnect(&selectedItem, &TextEditBox::focusChanged, this, &ATextEditTab::changeFocus);
        if (index == -1) { // not found
            std::cerr << "ATextEditTab::removeRect Unable to find element." << std::endl;
            return; // Abort
        }
        this->_scene->removeItem(&selectedItem);
        delete &selectedItem;
        this->changeFocus(false, nullptr); // Remove focus
    } catch (std::exception const &err) {
        std::cerr << "ATextEditTab::removeRect : fail to remove element. " << err.what() << std::endl;
    }
}

vector<BlockCluster> ATextEditTab::getClusters() const
{
    vector<BlockCluster> result;

    return result;
}

OCRPage ATextEditTab::getPage(size_t index)
{
    if (index >= (size_t)_pages.size()) {
        throw std::invalid_argument("ATextEditTab::getPage, invalid index");
    }
    OCRPage page = ImageViewer::getPage(index);

    page.clusters.clear();
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    if ((size_t)pageItems.size() <= index) {
        throw std::runtime_error("ATextEditTab::getPage, invalid _pageGroup size");
    }
    QGraphicsItem *pageItem = pageItems[index]; // Page image widget
    QRectF pageRect = pageItem->sceneBoundingRect();
    for (QGraphicsItem *item : _scene->items()) {
        TextEditBox *rect = dynamic_cast<TextEditBox *>(item);
        if (!rect)
            continue; // Not a TextEditBox
        if (rect->isOnArea(pageRect)) {
            page.clusters.push_back(rect->getData());
        }
    }
    return page;
}

void ATextEditTab::clearRects()
{
    this->_scene->clear();
}

void ATextEditTab::loadAPI()
{
    // Unfocus current rect
    this->changeFocus(false, nullptr);
}

void ATextEditTab::unload()
{
    NetEditTab::unload();
    this->changeFocus(false, nullptr); // Reset previous focus
}

/** PROTECTED **/

void ATextEditTab::foreachTextEditBox(std::function<void(TextEditBox &)> const &fun)
{
    QList<QGraphicsItem *> items = this->_scene->items();

    for ( QGraphicsItem *item : items) {
        TextEditBox *rect = dynamic_cast<TextEditBox *>(item);
        if (rect) {
            fun(*rect);
        }
    }
}

void ATextEditTab::keyPressEvent(QKeyEvent *event)
{
    ImageViewer::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete) {
        // remove currently selected TextEditBox
        this->removeRect();
    }
}

void ATextEditTab::mousePressEvent(QMouseEvent *)
{
    if (!_selectedItemId.isNull() && _focusedRectLostFocus == true) {
            _focusedRectLostFocus = false;
            this->changeFocus(false, nullptr);
    }
}

void ATextEditTab::doubleClickEvent(QMouseEvent *event)
{
    if (!_selectedItemId.isNull()) {
        return; // Editing existing rect. Not creating one.
    }
    const QPointF position = event->position(); // Position on the widget
    QPointF scenePos = this->_view->mapToScene(position.toPoint()); // Position on the scene

    this->createAreaRectAtCoord(scenePos);
}

TextEditBox &ATextEditTab::getRectFromId(QUuid const &id)
{
    QList<QGraphicsItem *> list = this->_scene->items();

    for (QGraphicsItem *item : list) {
        TextEditBox *rect = dynamic_cast<TextEditBox *>(item);
        if (rect && rect->getUuid() == id) {
            return *rect;
        }
    }
    throw std::invalid_argument("ATextEditTab::getRectFromId : "+id.toString().toStdString()+" not found.");
}

void ATextEditTab::createAreaRect(BlockCluster const &data, int pagePosY)
{
    auto *rect = new TextEditBox(data, _mode, pagePosY);

    this->_scene->addItem(rect);
    connect(rect, &TextEditBox::focusChanged, this, &ATextEditTab::changeFocus);
}

void ATextEditTab::setPagesEditAreas(vector<OCRPage> const &pages)
{
    QList<QGraphicsItem *> pageItems = _pageGroup->childItems();
    for (OCRPage const &page : pages) {
        for (BlockCluster const &cluster : page.clusters) {
            if (page.index >= pageItems.size()) {
                throw std::invalid_argument("ATextEditTab::setPagesEditAreas, corrupted page index.");
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

void ATextEditTab::changeFocus(bool focused, TextEditBox *rect)
{
    try {
        TextEditBox *selectedItem = (!_selectedItemId.isNull()) ? &this->getRectFromId(_selectedItemId) : nullptr;

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
        std::cerr << "ATextEditTab::changeFocus : fail to change focus. " << err.what() << std::endl;
        // Remove current focus:
        _selectedItemId = QUuid();
        emit this->sigRectFocusChanged(nullptr);
    }
}
