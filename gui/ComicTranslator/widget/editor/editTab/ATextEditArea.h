#ifndef ATEXTEDITAREA_H
#define ATEXTEDITAREA_H

#include "AEditArea.h"
#include "../editArea/EditAreaRect.h"
#include "include/IEditorArea.h"
#include "include/environment.h"

class ATextEditArea : public AEditArea, public IEditorArea
{
    Q_OBJECT
public:
    ATextEditArea(RectMode mode);

    virtual void setPages(vector<OCRPage> const &pages);

    void createAreaRectAtCoord(QPoint const &coord);

    void removeRect();

    std::vector<BlockCluster> getClusters() const;

protected:
    void keyPressEvent(QKeyEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);

    void createAreaRect(BlockCluster const& data);

protected slots:
    void changeFocus(bool focused, EditAreaRect &rect);

private:
    RectMode _mode;

protected:
    QList<EditAreaRect *> _rects;
    EditAreaRect *_focusedRect{nullptr};

};

#endif // ATEXTEDITAREA_H
