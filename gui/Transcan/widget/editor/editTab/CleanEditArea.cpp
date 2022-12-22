#include "CleanEditArea.h"
#include <functional>

using namespace std;

CleanEditArea::CleanEditArea(APIClient &client) : NetEditTab(client, CLEAN)
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
    //this->setPages(debug); // DEBUG
    //this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
}

/** PUBLIC **/

void CleanEditArea::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Load pages
    // Create clusters' select boxes
    for (OCRPage const &page : pages) {
        for (BlockCluster const &cluster : page.clusters) {
           this->createBlock(cluster);
        }
    }

}

std::vector<BlockCluster> CleanEditArea::getClusters() const
{
    std::vector<BlockCluster> result;
    QList<QGraphicsItem *> items = this->_scene->items();

    for (QGraphicsItem *item : items) {
        SelectAreaRect *rect = qgraphicsitem_cast<SelectAreaRect*>(item);
        if (rect && rect->isSelected()) {
            result.push_back(rect->getData());
        }
    }
    return result;
}

OCRPage CleanEditArea::getPage(size_t index)
{
    if (index >= (size_t)_pageItems.size()) {
        throw std::invalid_argument("CleanEditArea::getPage, invalid _pageItems size");
    }
    OCRPage page = ImageViewer::getPage(index);

    page.clusters.clear();
    for (QGraphicsItem *item : this->_scene->items()) {
        SelectAreaRect *rect = dynamic_cast<SelectAreaRect*>(item);
        if (rect != nullptr) {
            QGraphicsPixmapItem *pageItem = _pageItems[index]; // Get page image widget
            if (rect->isOnArea(pageItem->boundingRect())) {
                page.clusters.push_back(rect->getData());
            }
        }
    }
    return page;
}

void CleanEditArea::load(std::vector<OCRPage> const &pages)
{
    this->setPages(pages); // Apply pages to view
    this->setLoadingState(true);
    // API call
    for (OCRPage const &page: pages) {
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToClean(page, success, error); // TODO : to improve
    }
}

void CleanEditArea::loadPage(OCRPage const &page)
{
    this->updatePage(page);
    this->setLoadingState(false);
}

/** PRIVATE **/

void CleanEditArea::createBlock(BlockCluster const &cluster)
{
    auto *rect = new SelectAreaRect(cluster);

    this->_scene->addItem(rect);
}

