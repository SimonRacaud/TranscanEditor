#ifndef CLUSTERSTYLECONFIG_H
#define CLUSTERSTYLECONFIG_H

#include <QWidget>
#include <QSpinBox>
#include <QFontComboBox>
#include <QPushButton>
#include <QColorDialog>

#include "model/Page.h"
#include "include/models.h"

class ClusterStyleConfig : public QFrame
{
    Q_OBJECT
public:
    explicit ClusterStyleConfig(QString const &title);

    void setCluster(BlockCluster const &cluster);
    /**
     * @brief setConfig : Set values
     * @param config
     */
    void setConfig(RenderConfig const &config);

    /**
     * @brief getConfig : Export values
     * @return
     */
    RenderConfig getConfig();

signals:
    /**
     * @brief onUpdate : Emitted on any value update (widgets)
     */
    void onUpdate();

private:
    void onSelectColorButtonClicked(bool checked);

    void updateView();

    void connectEvents();
    void disconnectEvents();

private:
    RenderConfig _data;

    QSpinBox *_inLineHeight;
    QSpinBox *_inStrokeWidth;
    QFontComboBox *_inFont;
    QPushButton *_selectColorButton;
    QColorDialog *_colorSelect;
};

#endif // CLUSTERSTYLECONFIG_H