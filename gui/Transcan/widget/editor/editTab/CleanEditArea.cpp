#include "CleanEditArea.h"
#include <functional>

using namespace std;

CleanEditArea::CleanEditArea(APIClient &client) : NetEditTab(client, CLEAN)
{
}

/** PUBLIC **/

void CleanEditArea::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Load pages
    // Create clusters' select boxes
    QList<QGraphicsItem *> pageItems = this->_pageGroup->childItems();
    for (OCRPage const &page : pages) {
        const QGraphicsItem *pageItem = pageItems[page.index];
        for (BlockCluster const &cluster : page.clusters) {
            this->createBlock(cluster, pageItem->scenePos().y());
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
    QList<QGraphicsItem *> pageItems = this->_pageGroup->childItems();
    if (index >= (size_t)pageItems.size()) {
        throw std::invalid_argument("CleanEditArea::getPage, invalid number of page item");
    }
    OCRPage page = ImageViewer::getPage(index);

    page.clusters.clear();
    for (QGraphicsItem *item : this->_scene->items()) {
        SelectAreaRect *rect = dynamic_cast<SelectAreaRect*>(item);
        if (rect) {
            QGraphicsItem *pageItem = pageItems[index]; // Get page image widget
            if (rect->isOnArea(pageItem->sceneBoundingRect())) {
                page.clusters.push_back(rect->getData());
            }
        }
    }
    return page;
}

void CleanEditArea::load(std::vector<OCRPage> const &pages)
{
    try {
        this->setPages(pages); // Apply pages to view
    } catch (std::exception const &err) {
        std::cerr << "(load) An error occured: " << err.what() << std::endl;
    }
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

void CleanEditArea::createBlock(BlockCluster const &cluster, int pagePosY)
{
    auto *rect = new SelectAreaRect(cluster, pagePosY);

    this->_scene->addItem(rect);
}

