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

    virtual void loadAPI() override;
};

#endif // EXTRACTIONEDITAREA_H
