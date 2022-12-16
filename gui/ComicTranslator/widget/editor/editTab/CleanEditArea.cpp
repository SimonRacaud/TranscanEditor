#include "CleanEditArea.h"

using namespace std;

CleanEditArea::CleanEditArea()
{
    // -----------------------
    vector<OCRPage> debug = { // TODO: debug
    {
        .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg",
        .clusters = {
          BlockCluster { // DEBUG
              .blocks = {},
              .sentence = "",
              .polygon = {QPoint(100, 50), QPoint(300, 50), QPoint(300, 150), QPoint(100, 150)},
              .translation = "",
              .font = QFont(),
              .color = Qt::blue,
              .lineHeight = 100
          },
        },
    }};
    this->setPages(debug); // DEBUG
    //this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}

/** PUBLIC **/

void CleanEditArea::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Load pages
}

std::vector<BlockCluster> CleanEditArea::getClusters() const
{
    std::vector<BlockCluster> result(_rects.size());

    for (SelectAreaRect *rect : _rects) {
        if (rect->isSelected()) {
            result.push_back(rect->getData());
        }
    }
    return result;
}

OCRPage CleanEditArea::getPage(size_t index)
{
    OCRPage page = ImageViewer::getPage(index);

    page.clusters.clear();
    for (SelectAreaRect *rect : _rects) {
        if (_pageItems.size() >= index) {
            throw std::invalid_argument("CleanEditArea::getPage, invalid _pageItems size");
        }
        QGraphicsPixmapItem *pageItem = _pageItems[index];
        if (rect->isOnArea(pageItem->boundingRect()) == index) {
            page.clusters.push_back(rect->getData());
        }
    }
    return page;
}

/** PRIVATE **/

void CleanEditArea::createBlock(BlockCluster const &cluster)
{
    auto *rect = new SelectAreaRect(cluster);

    this->_scene->addItem(rect);
    this->_rects.append(rect);
}

