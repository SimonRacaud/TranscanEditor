#ifndef CLEANEDITAREA_H
#define CLEANEDITAREA_H

#include "AEditArea.h"
#include "../editArea/SelectAreaRect.h"

class CleanEditArea : public AEditArea
{
    Q_OBJECT
public:
    CleanEditArea();

    virtual void setPages(vector<OCRPage> const &pages);

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
