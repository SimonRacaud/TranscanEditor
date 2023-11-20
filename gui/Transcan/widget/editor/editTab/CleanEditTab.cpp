#include "CleanEditTab.h"
#include <functional>
#include <QScrollBar>
#include <QMouseEvent>
#include <QFileDialog>

using namespace std;

CleanEditTab::CleanEditTab(APIClient &client)
    : NetEditTab(client, CLEAN, true)
{
    connect(_view, &PageGraphicsView::onDoubleClick, this, &CleanEditTab::doubleClickEvent);
}

/** PUBLIC **/

void CleanEditTab::setPages(vector<OCRPage> const &pages)
{
    ImageViewer::setPages(pages); // Load pages
    // Create clusters' select boxes
    QList<QGraphicsItem *> pageItems = this->getPageGroup()->childItems();
    for (OCRPage const &page : pages) {
        const QGraphicsItem *pageItem = pageItems[page.index];
        for (BlockCluster const &cluster : page.clusters) {
            this->createBlock(cluster, pageItem->scenePos().y());
        }
    }

}

std::vector<BlockCluster> CleanEditTab::getClusters() const
{
    std::vector<BlockCluster> result;
    QList<QGraphicsItem *> items = this->_scene->items();

    for (QGraphicsItem *item : items) {
        SelectAreaBox *rect = qgraphicsitem_cast<SelectAreaBox*>(item);
        if (rect && rect->isSelected()) {
            result.push_back(rect->getData());
        }
    }
    return result;
}

OCRPage CleanEditTab::getPage(size_t index)
{
    QList<QGraphicsItem *> pageItems = this->getPageGroup()->childItems();
    if (index >= (size_t)pageItems.size()) {
        throw std::invalid_argument("CleanEditArea::getPage, invalid number of page item");
    }
    OCRPage page = ImageViewer::getPage(index);

    page.clusters.clear();
    for (QGraphicsItem *item : this->_scene->items()) {
        SelectAreaBox *rect = dynamic_cast<SelectAreaBox*>(item);
        if (rect) {
            QGraphicsItem *pageItem = pageItems[index]; // Get page image widget
            if (rect->isOnArea(pageItem->sceneBoundingRect())) {
                page.clusters.push_back(rect->getData());
            }
        }
    }
    return page;
}

void CleanEditTab::loadAPI()
{
    this->setLoadingState(true);

    for (size_t i = 0; i < _pages.size(); i++) {
        OCRPage const &page = this->getPage(i);
        NetCallback success = bind(&IEditTab::loadPage, this, std::placeholders::_1);
        NetErrCallback error = bind(&NetEditTab::netError, this, std::placeholders::_1);
        this->_api.sendToClean(page, success, error); // TODO : to improve - network
    }
}

/** SLOTS **/

void CleanEditTab::slotReplacePage(QString const &filePath)
{
    // Detect the current page
    QPoint viewCenter = this->_view->rect().center();
    QPointF targetPoint = this->_view->mapToScene(viewCenter);
    int targetPageIndex = this->getPageIndexAtCoord(targetPoint);

    if (targetPageIndex == -1) {
        std::cerr << "Info: CleanEditArea::slotReplacePage Cannot replace page, not current page." << std::endl;
        return; // Abort
    }
    // Update the current page
    try {
        OCRPage page = this->getPage(targetPageIndex);
        page.cleanImagePath = filePath;
        this->updatePage(page);
    } catch (std::exception const &err) {
        std::cerr << "Error CleanEditArea::slotReplacePage, unable to update page's image." << std::endl;
    }
}

/** PRIVATE **/

void CleanEditTab::createBlock(BlockCluster const &cluster, int pagePosY)
{
    auto *rect = new SelectAreaBox(cluster, pagePosY);

    this->_scene->addItem(rect);
}

void CleanEditTab::replacePageAtCoord(QPointF const &position)
{
    int targetPageIndex = this->getPageIndexAtCoord(position);
    if (targetPageIndex == -1) {
        return; // Abort
    }
    const QString &filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr(IMG_SELECT_FILTER));

    if (!filePath.isEmpty()) {
        this->slotReplacePage(filePath);
    }
}

 void CleanEditTab::doubleClickEvent(QMouseEvent *event)
 {
    const QPointF &coord = event->position();
    bool isOnRect = false;

    // Check of coord position is located on a SelectAreaBox.
    // We only want to trigger an action if the click is directly located on a page.
    for (QGraphicsItem *item : this->_scene->items()) {
        SelectAreaBox *rect = dynamic_cast<SelectAreaBox*>(item);
        if (rect) {
            if (rect->sceneBoundingRect().contains(coord)) {
                isOnRect = true;
                break;
            }
        }
    }
    if (!isOnRect) {
        this->replacePageAtCoord(coord);
    }
 }

int CleanEditTab::getPageIndexAtCoord(QPointF const &coord)
{
    QList<QGraphicsItem *> pageItems = this->getPageGroup()->childItems();
    int pageIndex = -1;

    for (qsizetype i = 0; i < pageItems.size(); i++) {
        if (pageItems[i]->sceneBoundingRect().contains(coord)) {
            pageIndex = i;
            break;
        }
    }
    return pageIndex;
}

