#ifndef CLEANEDITAREA_H
#define CLEANEDITAREA_H

#include "NetEditTab.h"
#include "widget/editor/editArea/SelectAreaRect.h"

class CleanEditArea : public NetEditTab
{
    Q_OBJECT
public:
    CleanEditArea(APIClient &client);

    virtual void setPages(std::vector<OCRPage> const &pages) override;

    /**
     * @brief getClusters : return the selected areas
     */
    std::vector<BlockCluster> getClusters() const;

    /**
     * @brief getPage Export SelectAreaRect list as a list of OCRBlock
     * @param index
     * @return
     */
    virtual OCRPage getPage(size_t index) override;

    virtual void loadAPI() override;
private:
    void createBlock(BlockCluster const &cluster, int pagePosY);

private:
};

#endif // CLEANEDITAREA_H
