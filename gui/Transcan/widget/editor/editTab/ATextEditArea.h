#ifndef ATEXTEDITAREA_H
#define ATEXTEDITAREA_H

#include "include/IEditTab.h"
#include "include/environment.h"
#include "widget/editor/editArea/EditAreaRect.h"
#include "widget/imageViewer/ImageViewer.h"

class ATextEditArea : public ImageViewer, public IEditTab
{
    Q_OBJECT
public:
    ATextEditArea(RectMode mode);

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    void createAreaRectAtCoord(QPoint const &coord);

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

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;
    /**
     * @brief doubleClickEvent Mouse event on Graphic View
     * @param event
     */
    void doubleClickEvent(QMouseEvent *event);

    void createAreaRect(BlockCluster const& data);

    void setPagesEditAreas(std::vector<OCRPage> const &pages);

protected slots:
    void changeFocus(bool focused, EditAreaRect &rect);

private:
    RectMode _mode;

protected:
    QList<EditAreaRect *> _rects;
    EditAreaRect *_focusedRect{nullptr};

};

#endif // ATEXTEDITAREA_H
