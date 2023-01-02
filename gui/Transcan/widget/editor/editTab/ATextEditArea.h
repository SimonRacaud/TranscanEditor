#ifndef ATEXTEDITAREA_H
#define ATEXTEDITAREA_H

#include "include/environment.h"
#include "widget/editor/editArea/EditAreaRect.h"
#include "NetEditTab.h"

class ATextEditArea : public NetEditTab
{
    Q_OBJECT
public:
    ATextEditArea(APIClient &client, ImageMode modeImg, RectMode mode, bool autoReload);

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    void createAreaRectAtCoord(QPointF const &coord);

    void removeRect();

    std::vector<BlockCluster> getClusters() const;

    /**
     * @brief getPage Convert local EditAreaRect list to BlockClusters
     * @param index Index of the page to export
     * @return
     */
    virtual OCRPage getPage(size_t index) override;

    /**
     * @brief clearRects : Remove every rects from the view
     */
    void clearRects();

    virtual void loadAPI() override;

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    virtual void mousePressEvent(QMouseEvent *event) override;
    /**
     * @brief doubleClickEvent Mouse event on Graphic View
     * @param event
     */
    void doubleClickEvent(QMouseEvent *event);

    void createAreaRect(BlockCluster const& data, int pagePosY);

    void setPagesEditAreas(std::vector<OCRPage> const &pages);

    EditAreaRect &getRectFromId(QUuid const &id);

protected slots:
    void changeFocus(bool focused, EditAreaRect *rect);

signals:
    /**
     * @brief sigRectFocusChanged
     *  - focused on click
     *  - lose focus when another rect is focussed or on click on the EditArea
     * @param rect
     */
    void sigRectFocusChanged(EditAreaRect *rect);

private:
    RectMode _mode;

protected:
    QUuid _selectedItemId;
    bool _focusedRectLostFocus{false};

};

#endif // ATEXTEDITAREA_H
