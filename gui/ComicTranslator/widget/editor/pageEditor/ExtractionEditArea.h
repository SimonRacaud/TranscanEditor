#ifndef EXTRACTIONEDITAREA_H
#define EXTRACTIONEDITAREA_H

#include "../AEditArea.h"
#include "../editArea/EditAreaRect.h"

class ExtractionEditArea : public AEditArea
{
    Q_OBJECT
public:
    ExtractionEditArea();

    virtual void setPages(vector<OCRPage> const &pages);

private:
    EditAreaRect *_rect;
};

#endif // EXTRACTIONEDITAREA_H
