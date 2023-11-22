#ifndef EXTRACTIONEDITTAB_H
#define EXTRACTIONEDITTAB_H

#include "ATextEditTab.h"
#include "widget/editor/box/TextEditBox.h"
#include "model/Page.h"

/**
 * @brief The ExtractionEditTab class : To extract pages' old text
 */
class ExtractionEditTab : public ATextEditTab
{
    Q_OBJECT
public:
    ExtractionEditTab(APIClient &client);

    virtual void loadAPI() override;
};

#endif // EXTRACTIONEDITTAB_H
