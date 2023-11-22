#ifndef SAVEEDITTAB_H
#define SAVEEDITTAB_H

#include "NetEditTab.h"

/**
 * @brief The SaveEditTab class : Render pages to images and display the result
 */
class SaveEditTab : public NetEditTab
{
    Q_OBJECT
public:
    SaveEditTab(APIClient &client);

    virtual void loadAPI() override;

public slots:
    void onUpdateProjectDestinationPath(QString const&path);
    void onExport();

private:
};

#endif // SAVEEDITTAB_H
