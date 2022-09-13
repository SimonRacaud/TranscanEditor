#include "CleanEditArea.h"

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
              .box = QRect(100, 50, 200, 100),
              .polygon = {},
              .translation = "",
              .font = QFont(),
              .color = Qt::blue,
              .line_height = 100,
          },
        },
    }};
    this->setPages(debug); // DEBUG
    this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}

/** PUBLIC **/

void CleanEditArea::setPages(vector<OCRPage> const &pages)
{
    AEditArea::setPages(pages); // Load pages
    for (OCRPage const &page : pages) {
        for (BlockCluster const &cluster : page.clusters) {
            this->createBlock(cluster);
        }
    }
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

/** PRIVATE **/

void CleanEditArea::createBlock(BlockCluster const &cluster)
{
    auto *rect = new SelectAreaRect(cluster);

    this->_scene->addItem(rect);
    this->_rects.append(rect);
}
