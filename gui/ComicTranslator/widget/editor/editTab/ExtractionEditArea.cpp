#include "ExtractionEditArea.h"
#include "include/model/OCRPage.h"
#include <vector>

using namespace std;

ExtractionEditArea::ExtractionEditArea() : AEditArea()
{
    // -----------------------
    vector<OCRPage> debug = { // TODO: debug
        {
            .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg",
            .clusters = {
              BlockCluster { // DEBUG
                  .blocks = {},
                  .sentence = "Hello World",
                  .box = QRect(0, 0, 200, 100),
                  .polygon = {QPoint(0, 0), QPoint(200, 0), QPoint(200, 100), QPoint(0, 100)},
                  .translation = "LorenIpsum",
                  .font = QFont(),
                  .color = Qt::blue,
                  .line_height = 100,
              },
              BlockCluster { // DEBUG
                  .blocks = {},
                  .sentence = "Hello World",
                  .box = QRect(100, 200, 200, 100),
                  .polygon = {},
                  .translation = "LorenIpsum",
                  .font = QFont(),
                  .color = Qt::blue,
                  .line_height = 100,
              },
            }
        },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
    };
    this->setPages(debug); // DEBUG
    this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}

/** PUBLIC FUNCTIONS **/

void ExtractionEditArea::setPages(vector<OCRPage> const &pages)
{
    AEditArea::setPages(pages); // Display page images
    for (OCRPage const &page : pages) {
        for (BlockCluster const &cluster : page.clusters) {
            this->createAreaRect(cluster);
        }
    }
}

void ExtractionEditArea::createAreaRectAtCoord(QPoint const &coord)
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

void ExtractionEditArea::removeRect()
{
    if (_focusedRect) {
        disconnect(_focusedRect, &EditAreaRect::focusChanged, this, &ExtractionEditArea::changeFocus);
        int index = _rects.indexOf(_focusedRect);
        if (index == -1) { // not found
            _focusedRect = nullptr;
            std::cerr << "ExtractionEditArea::removeRect Unable to find element." << std::endl;
            return; // Abort
        }
        _rects.removeAt(index);
        this->_scene->removeItem(_focusedRect);
        delete _focusedRect;
        _focusedRect = nullptr;
    }
}

vector<BlockCluster> ExtractionEditArea::getClusters() const
{
    vector<BlockCluster> result(_rects.size());

    for (EditAreaRect *rect : _rects) {
        result.push_back(rect->getData());
    }
    return result;
}

/** PROTECTED **/

void ExtractionEditArea::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete) {
        // remove currently selected EditAreaRect
        this->removeRect();
    }
}

void ExtractionEditArea::mouseDoubleClickEvent(QMouseEvent *event)
{
    QWidget::mouseDoubleClickEvent(event);
    const QPointF &position = event->position(); // Position on the screen
    int sliderPosY = this->_view->verticalScrollBar()->sliderPosition(); // scroll bar shift

    this->createAreaRectAtCoord(QPoint(position.x(), position.y() + sliderPosY));
}

void ExtractionEditArea::createAreaRect(BlockCluster const &data)
{
    auto *rect = new EditAreaRect(data, RectMode::EDIT_SENT);

    this->_scene->addItem(rect);
    this->_rects.append(rect);
    connect(rect, &EditAreaRect::focusChanged, this, &ExtractionEditArea::changeFocus);
}


/** SLOTS **/

void ExtractionEditArea::changeFocus(bool focused, EditAreaRect &rect)
{
    if (focused) {
        this->_focusedRect = &rect;
    } else if (_focusedRect == &rect) {
        this->_focusedRect = nullptr;
    }
}
