#ifndef APROPERTYTAB_H
#define APROPERTYTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "widget/misc/ZoomWidget.h"

using FuncNetCall = std::function<void()>;

class APropertyTab : public QWidget
{
    Q_OBJECT
public:
    /**
     * @brief APropertyTab
     * @param reloadFunc : Function called on click of the reload button
     * @param parent
     */
    explicit APropertyTab(FuncNetCall &reloadFunc, QWidget *parent = nullptr);

    void onNetworkError();

public slots:
    void unlockReloadButton();
    void setZoom(float value);

protected:
    virtual void fillHelp() = 0;
    virtual void initProperties() = 0;

signals:
    void nextStep();
    void sigChangeZoom(float value);

private:
    FuncNetCall _netCallback;
    QLabel *_title;
    QLabel *_helpTitle;
    QScrollArea *_propScollArea;

protected:
    QPushButton *_reloadButton;
    QPushButton *_nextButton;
    QLabel *_help;
    QVBoxLayout *_propertiesLayout;
    ZoomWidget *_zoomWidget;
};

#endif // APROPERTYTAB_H
