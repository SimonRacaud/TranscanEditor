#ifndef SAVEEDITAREA_H
#define SAVEEDITAREA_H

#include "NetEditTab.h"

class SaveEditArea : public NetEditTab
{
    Q_OBJECT
public:
    SaveEditArea(APIClient &client);

    virtual void load(std::vector<OCRPage> const &pages = {});
    virtual void loadPage(OCRPage const &page);
    virtual void unload();
private:
};

#endif // SAVEEDITAREA_H
