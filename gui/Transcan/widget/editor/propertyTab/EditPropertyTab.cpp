#include "EditPropertyTab.h"
#include <QVBoxLayout>

EditPropertyTab::EditPropertyTab(FuncNetCall &reloadFunc, QWidget *parent)
    : APropertyTab(reloadFunc, parent)
{
    this->fillHelp();
    this->initProperties();
    this->_reloadButton->setText(tr("Translate"));

    connect(_defaultStyleConfig, &ClusterStyleConfig::onUpdate, this, &EditPropertyTab::onUpdateGlobalClusterStyle);
    connect(_selectedClusterConfig, &ClusterStyleConfig::onUpdate, this, &EditPropertyTab::onUpdateSelectedClusterStyle);
}

void EditPropertyTab::fillHelp()
{
    this->_help->setText(tr(
                "Edit:\n"
                "\n"
                "Actions:\n"
                "- Edit generated translation\n"
                "- Edit test style\n"
                "\n"
                "Controls:\n"
                "- Click on a box to select it\n"
                "- Double click to edit its content\n"
                ));
}

void EditPropertyTab::initProperties()
{
    this->_defaultStyleConfig = new ClusterStyleConfig(tr("Default configuration:"));
    this->_selectedClusterConfig = new ClusterStyleConfig(tr("Selected box:"));

    this->_propertiesLayout->addWidget(_defaultStyleConfig);
    this->_propertiesLayout->addWidget(_selectedClusterConfig);

    this->_defaultStyleConfig->setEnabled(false); // Not received any config object yet
    this->_selectedClusterConfig->hide();
}

void EditPropertyTab::updateProjectConfig(ProjectConfig const &config)
{
    this->_defaultStyleConfig->setEnabled(true); // Unlock
    this->_defaultStyleConfig->setConfig(config.renderConf);
}

/* SLOTS */

void EditPropertyTab::onFocusCluster(RenderConfig const &style)
{
    this->_selectedClusterConfig->show();
    this->_selectedClusterConfig->setConfig(style);
}

void EditPropertyTab::onUnfocusCluster()
{
    this->_selectedClusterConfig->hide();
}

/*Privates Slots*/

void EditPropertyTab::onUpdateSelectedClusterStyle()
{
    // flush update
    emit this->sigUpdateClusterStyle(_selectedClusterConfig->getConfig());
}

void EditPropertyTab::onUpdateGlobalClusterStyle()
{
    // flush update
    RenderConfig const &config = _defaultStyleConfig->getConfig();
    emit this->sigUpdateAllClusterStyle(config);
    if (this->_selectedClusterConfig->isEnabled()) {
        // The selected cluster will have its config updated.
        // So update the widget values shown as well.
        _selectedClusterConfig->setConfig(config);
    }
}
