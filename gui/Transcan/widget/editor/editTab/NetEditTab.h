#ifndef NETEDITTAB_H
#define NETEDITTAB_H

#include "widget/imageViewer/ImageViewer.h"
#include "include/IEditTab.h"

#include "net/APIClient.h"
#include "include/models.h"

/**
 * @brief The NetEditTab class
 * Give the ability to communication for the core to the Editor's tabs
 */
class NetEditTab : public ImageViewer, public IEditTab
{
public:
    NetEditTab(APIClient &client, ImageMode mode);
    virtual ~NetEditTab();

    void netError(QString const &message);

    void setConfig(ProjectConfig const &config);

    virtual void unload() override;

    virtual void setLoadingState(bool enable) override;

protected:
    APIClient &_api;
    const ProjectConfig *_config{nullptr};
};

#endif // NETEDITTAB_H
