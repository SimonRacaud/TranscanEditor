#ifndef EDITPROPERTYTAB_H
#define EDITPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"
#include "include/environment.h"
#include "edit/ClusterStyleConfig.h"
#include "include/models.h"

#include <QFontComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>

class EditPropertyTab : public APropertyTab
{
    Q_OBJECT
public:
    /**
     * @brief EditPropertyTab
     * @param reloadFunc : Called to relaunch a call to the Core
     * @param parent
     */
    EditPropertyTab(FuncNetCall &reloadFunc, QWidget *parent = nullptr);

    /**
     * @brief updateProjectConfig : Set the default values for _defaultStyleConfig
     * @param config
     */
    void updateProjectConfig(ProjectConfig const &config);

public slots:
    /**
     * @brief onFocusCluster : When an EditRect is focused with its current values
     * @param cluster
     */
    void onFocusCluster(RenderConfig const &style);
    /**
     * @brief onUnfocusCluster : When an EditRect lose its focus
     */
    void onUnfocusCluster();

signals:
    /**
     * @brief sigUpdateClusterStyle : To update the style of an EditRect
     * @param style
     */
    void sigUpdateClusterStyle(RenderConfig const &style);
    /**
     * @brief sigUpdateAllClusterStyle : To update the style of every EditRect of the scene
     * @param style
     */
    void sigUpdateAllClusterStyle(RenderConfig const &style);

private:
    virtual void fillHelp();
    virtual void initProperties();

private slots:
    /**
     * @brief onUpdateSelectedClusterStyle : when the user edit a value (_selectedClusterConfig)
     */
    void onUpdateSelectedClusterStyle();
    /**
     * @brief onUpdateGlobalClusterStyle : when the user edit a value (_defaultStyleConfig)
     */
    void onUpdateGlobalClusterStyle();

private:
    ClusterStyleConfig *_defaultStyleConfig;
    ClusterStyleConfig *_selectedClusterConfig;
};

#endif // EDITPROPERTYTAB_H
