#ifndef CLEANPROPERTYTAB_H
#define CLEANPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

class CleanPropertyTab : public APropertyTab
{
public:
    CleanPropertyTab(QWidget *parent = nullptr);

private:
    virtual void fillHelp();
    virtual void initProperties();

private:
    QPushButton *_fileSelect;
    QLabel *_fileSelectLabel;
    QLabel *_previewWidget;
    QPixmap _previewImg;
    QPushButton *_applyButton;
};

#endif // CLEANPROPERTYTAB_H
