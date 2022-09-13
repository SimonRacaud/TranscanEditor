#ifndef APROPERTYTAB_H
#define APROPERTYTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "include/environment.h"
#include "../editTab/AEditArea.h"

class APropertyTab : public QWidget
{
    Q_OBJECT
public:
    explicit APropertyTab(QWidget *parent = nullptr);

protected:
    virtual void fillHelp() = 0;
    virtual void initProperties() = 0;

signals:
    void nextStep();

private:
    QLabel *_title;
    QLabel *_helpTitle;
    QScrollArea *_propScollArea;

protected:
    QPushButton *_nextButton;
    QLabel *_help;
    QVBoxLayout *_propertiesLayout;
};

#endif // APROPERTYTAB_H
