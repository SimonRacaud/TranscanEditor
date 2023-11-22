#ifndef NETEDITTAB_H
#define NETEDITTAB_H

#include "widget/imageViewer/ImageViewer.h"
#include "IEditTab.h"

#include "net/APIClient.h"
#include "model/configModels.h"

/**
 * @brief The NetEditTab class
 * Allow a page viewer to communicate with the CORE
 */
class NetEditTab : public ImageViewer, public IEditTab
{
    Q_OBJECT
public:
    /**
     * @brief NetEditTab
     * @param client Client to access the core
     * @param mode Which path will be used from OCRPage
     * @param autoReload If the pages will be (re)processed by the core when changing tab.
     */
    NetEditTab(APIClient &client, ImageMode mode, bool autoReload = true);
    virtual ~NetEditTab();

    void netError(QString const &message);

    void setConfig(shared_ptr<ProjectConfig> config);

    virtual void unload() override;
    virtual void load(std::vector<OCRPage> const &pages = {}) override;
    virtual void loadPage(OCRPage const &page) override;

    virtual void setLoadingState(bool enable) override;

signals:
    void allAPIRequestsCompleted();

protected:
    APIClient &_api;
    const bool _autoReload;
    shared_ptr<ProjectConfig> _config;
};

#endif // NETEDITTAB_H
