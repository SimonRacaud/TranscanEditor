#ifndef EXTRACTIONEDITAREA_H
#define EXTRACTIONEDITAREA_H

#include "../AEditArea.h"
#include "../editArea/EditAreaRect.h"
#include "include/IEditorArea.h"
#include "include/environment.h"

#include "../editArea/SelectAreaRect.h" // DEBUG

class ExtractionEditArea : public AEditArea, public IEditorArea
{
    Q_OBJECT
public:
    ExtractionEditArea();

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
    QList<EditAreaRect *> _rects;
    EditAreaRect *_focusedRect{nullptr};
};

#endif // EXTRACTIONEDITAREA_H
