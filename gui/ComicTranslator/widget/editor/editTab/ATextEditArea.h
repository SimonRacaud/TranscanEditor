#ifndef ATEXTEDITAREA_H
#define ATEXTEDITAREA_H

#include "include/ITextEditArea.h"
#include "include/environment.h"
#include "widget/editor/editArea/EditAreaRect.h"
#include "widget/imageViewer/ImageViewer.h"

class ATextEditArea : public ImageViewer, public ITextEditArea
{
    Q_OBJECT
public:
    ATextEditArea(RectMode mode);

    virtual void setPages(std::vector<OCRPage> const &pages);

    void createAreaRectAtCoord(QPoint const &coord);

    void removeRect();

    std::vector<BlockCluster> getClusters() const;

protected:
    virtual void keyPressEvent(QKeyEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

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
