#ifndef EXTRACTIONEDITAREA_H
#define EXTRACTIONEDITAREA_H

#include "ATextEditArea.h"
#include "widget/editor/editArea/EditAreaRect.h"
#include "include/environment.h"
#include "include/model/OCRPage.h"

class ExtractionEditArea : public ATextEditArea
{
    Q_OBJECT
public:
    ExtractionEditArea();

    virtual void loadPage(OCRPage const &page);
    virtual void load(std::vector<OCRPage> const &pages = {});
    virtual void unload();
};

#endif // EXTRACTIONEDITAREA_H
