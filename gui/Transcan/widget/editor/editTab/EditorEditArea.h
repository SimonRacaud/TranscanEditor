#ifndef EDITOREDITAREA_H
#define EDITOREDITAREA_H

#include "ATextEditArea.h"

class EditorEditArea : public ATextEditArea
{
    Q_OBJECT
public:
    EditorEditArea(APIClient &client);

    virtual void load(std::vector<OCRPage> const &pages = {});
    virtual void loadPage(OCRPage const &page);
    virtual void unload();
};

#endif // EDITOREDITAREA_H
