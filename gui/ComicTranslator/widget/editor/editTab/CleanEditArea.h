#ifndef CLEANEDITAREA_H
#define CLEANEDITAREA_H

#include "widget/imageViewer/ImageViewer.h"
#include "widget/editor/editArea/SelectAreaRect.h"

class CleanEditArea : public ImageViewer
{
    Q_OBJECT
public:
    CleanEditArea();

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    /**
     * @brief getClusters : return the selected areas
     */
    std::vector<BlockCluster> getClusters() const;

private:
    void createBlock(BlockCluster const &cluster);

private:
    QList<SelectAreaRect *> _rects;
};

#endif // CLEANEDITAREA_H
