#ifndef SAVEEDITAREA_H
#define SAVEEDITAREA_H

#include "NetEditTab.h"

class SaveEditArea : public NetEditTab
{
    Q_OBJECT
public:
    SaveEditArea(APIClient &client);

    virtual void loadAPI() override;
private:
};

#endif // SAVEEDITAREA_H
