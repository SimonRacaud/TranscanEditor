#ifndef CLEANEDITTAB_H
#define CLEANEDITTAB_H

#include "NetEditTab.h"
#include "widget/editor/box/SelectAreaBox.h"

/**
 * @brief The CleanEditTab class : To remove text on pages
 */
class CleanEditTab : public NetEditTab
{
    Q_OBJECT
public:
    CleanEditTab(APIClient &client);

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    /**
     * @brief getClusters : return the selected areas
     */
    std::vector<BlockCluster> getClusters() const;

    /**
     * @brief getPage Export SelectAreaBox list as a list of OCRBlock
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
    int getPageIndexAtCoord(QPointF const &coord);

private:
};

#endif // CLEANEDITTAB_H
