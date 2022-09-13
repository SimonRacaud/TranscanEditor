#include "ATextEditArea.h"

ATextEditArea::ATextEditArea(RectMode mode) : AEditArea(), _mode(mode)
{
}

/** PUBLIC **/

void ATextEditArea::setPages(vector<OCRPage> const &pages)
{
    AEditArea::setPages(pages); // Display page images
    for (OCRPage const &page : pages) {
        for (BlockCluster const &cluster : page.clusters) {
            this->createAreaRect(cluster);
        }
    }
}

void ATextEditArea::createAreaRectAtCoord(QPoint const &coord)
{
    const QPoint size(DEF_EDITAREA_SIZE);
    const BlockCluster defaultData = {
        .blocks = {},
        .sentence = "Text",
        .box = QRect(coord.x() - (size.x() / 2), coord.y() - (size.y() / 2), DEF_EDITAREA_SIZE),
        .polygon = {},
        .translation = "",
        .font = QFont(),
        .color = Qt::black,
        .line_height = DEF_LINE_HEIGHT,
    };
    this->createAreaRect(defaultData);
}

void ATextEditArea::removeRect()
{
    if (_focusedRect) {
        disconnect(_focusedRect, &EditAreaRect::focusChanged, this, &ATextEditArea::changeFocus);
        int index = _rects.indexOf(_focusedRect);
        if (index == -1) { // not found
            _focusedRect = nullptr;
            std::cerr << "ATextEditArea::removeRect Unable to find element." << std::endl;
            return; // Abort
        }
        _rects.removeAt(index);
        this->_scene->removeItem(_focusedRect);
        delete _focusedRect;
        _focusedRect = nullptr;
    }
}

vector<BlockCluster> ATextEditArea::getClusters() const
{
    vector<BlockCluster> result(_rects.size());

    for (EditAreaRect *rect : _rects) {
        result.push_back(rect->getData());
    }
    return result;
}

/** PROTECTED **/

void ATextEditArea::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete) {
        // remove currently selected EditAreaRect
        this->removeRect();
    }
}

void ATextEditArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    const QPointF &position = event->position(); // Position on the screen
    int sliderPosY = this->_view->verticalScrollBar()->sliderPosition(); // scroll bar shift

    this->createAreaRectAtCoord(QPoint(position.x(), position.y() + sliderPosY));
}

void ATextEditArea::createAreaRect(BlockCluster const &data)
{
    auto *rect = new EditAreaRect(data, _mode);

    this->_scene->addItem(rect);
    this->_rects.append(rect);
    connect(rect, &EditAreaRect::focusChanged, this, &ATextEditArea::changeFocus);
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
