#ifndef SAVEEDITAREA_H
#define SAVEEDITAREA_H

#include "NetEditTab.h"

class SaveEditArea : public NetEditTab
{
    Q_OBJECT
public:
    SaveEditArea(APIClient &client);

    virtual void loadAPI() override;

public slots:
    void onUpdateProjectDestinationPath(QString const&path);
    void onExport();

private:
};

#endif // SAVEEDITAREA_H
