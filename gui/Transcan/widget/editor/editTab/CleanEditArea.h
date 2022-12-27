#ifndef CLEANEDITAREA_H
#define CLEANEDITAREA_H

#include "NetEditTab.h"
#include "widget/editor/editArea/SelectAreaRect.h"

class CleanEditArea : public NetEditTab
{
    Q_OBJECT
public:
    CleanEditArea(APIClient &client);

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    /**
     * @brief getClusters : return the selected areas
     */
    std::vector<BlockCluster> getClusters() const;

    /**
     * @brief getPage Export SelectAreaRect list as a list of OCRBlock
     * @param index
     * @return
     */
    virtual OCRPage getPage(size_t index) override;

    virtual void loadAPI() override;

public slots:
    /**
     * @brief slotReplacePage Replac the current page image by the one given
     * as parameter
     * @param filePath
     */
    void slotReplacePage(QString const &filePath);

private:
    void createBlock(BlockCluster const &cluster, int pagePosY);

    /**
     * @brief replacePageAtCoord : Replace a page image (clean) for the page
     *  located at position
     * @param position
     */
    void replacePageAtCoord(QPointF const &position);

    /**
     * @brief doubleClickEvent : On a double click on the view
     * @param event
     */
    void doubleClickEvent(QMouseEvent *event);

    /**
     * @brief getPageIndexAtCoord : Return the index of the page (_pages list)
     *  located at coord on the scene.
     * @param coord
     * @return -1 if no page at this coordinate.
     */
    int getPageIndexAtCoord(QPointF const &coord) const;

private:
};

#endif // CLEANEDITAREA_H
