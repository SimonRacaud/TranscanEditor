#ifndef APROPERTYTAB_H
#define APROPERTYTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>
#include <QScrollArea>

#include "include/environment.h"

class APropertyTab : public QWidget
{
    Q_OBJECT
public:
    explicit APropertyTab(QWidget *parent = nullptr);

protected:
    virtual void fillHelp() = 0;
    virtual void initProperties() = 0;

signals:

private:
    QLabel *_title;
    QLabel *_helpTitle;
    QPushButton *_nextButton;
    QScrollArea *_propScollArea;

protected:
    QLabel *_help;
    QVBoxLayout *_propertiesLayout;
};

#endif // APROPERTYTAB_H
