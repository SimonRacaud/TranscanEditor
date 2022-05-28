#ifndef APROPERTYTAB_H
#define APROPERTYTAB_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>

#include "include/environment.h"

class APropertyTab : public QWidget
{
    Q_OBJECT
public:
    explicit APropertyTab(QWidget *parent = nullptr);

signals:

private:
    QLabel *_title;
    QLabel *_helpTitle;
    QLabel *_help;
    QPushButton *_nextButton;
};

#endif // APROPERTYTAB_H
