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

    /**
     * @brief netError : Called in case of network request error on current tab
     * @param message : Network error message
     */
    void netError(QString const &message);

    /**
     * @brief setConfig : Set Project global configuration
     * @param config
     */
    void setConfig(shared_ptr<ProjectConfig> config);

    /**
     * @brief Called on tab switch (close). Cancel pending API calls.
     */
    virtual void unload() override;
    /**
     * @brief Called on tab switch (open). Load pages on view, launch api calls.
     */
    virtual void load(std::vector<OCRPage> const &pages = {}) override;
    /**
     * @brief modify specific page on view
     */
    virtual void loadPage(OCRPage const &page) override;
    /**
     * @brief Show or hide loading animation on view
     */
    virtual void setLoadingState(bool enable) override;

signals:
    /**
     * @brief allAPIRequestsCompleted : Emitted when every API requests have been processed
     */
    void allAPIRequestsCompleted();

protected:
    APIClient &_api; // Network client for the current tab
    const bool _autoReload; // Automatically start API call on tab opening
    shared_ptr<ProjectConfig> _config;
};

#endif // NETEDITTAB_H
