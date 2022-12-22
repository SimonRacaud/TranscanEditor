#ifndef EXTRACTIONEDITAREA_H
#define EXTRACTIONEDITAREA_H

#include "ATextEditArea.h"
#include "widget/editor/editArea/EditAreaRect.h"
#include "include/environment.h"
#include "model/Page.h"

class ExtractionEditArea : public ATextEditArea
{
    Q_OBJECT
public:
    ExtractionEditArea(APIClient &client);

    virtual void loadPage(OCRPage const &page);
    virtual void load(std::vector<OCRPage> const &pages = {});
};

#endif // EXTRACTIONEDITAREA_H
