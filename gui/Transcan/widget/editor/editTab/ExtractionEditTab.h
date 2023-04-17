#ifndef EXTRACTIONEDITTAB_H
#define EXTRACTIONEDITTAB_H

#include "ATextEditTab.h"
#include "widget/editor/box/TextEditBox.h"
#include "include/environment.h"
#include "model/Page.h"

class ExtractionEditTab : public ATextEditTab
{
    Q_OBJECT
public:
    ExtractionEditTab(APIClient &client);

    virtual void loadAPI() override;
};

#endif // EXTRACTIONEDITTAB_H
