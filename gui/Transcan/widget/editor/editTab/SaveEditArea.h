#ifndef SAVEEDITAREA_H
#define SAVEEDITAREA_H

#include "widget/imageViewer/ImageViewer.h"

class SaveEditArea : public ImageViewer
{
    Q_OBJECT
public:
    SaveEditArea();

    virtual void load(std::vector<OCRPage> const &pages = {});
    virtual void loadPage(OCRPage const &page);
    virtual void unload();
private:
};

#endif // SAVEEDITAREA_H
