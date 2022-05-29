#ifndef SAVEPROPERTYTAB_H
#define SAVEPROPERTYTAB_H

#include "widget/editor/APropertyTab.h"
#include "include/environment.h"

#include <QLabel>
#include <QPushButton>

class SavePropertyTab : public APropertyTab
{
public:
    SavePropertyTab(QWidget *parent = nullptr);

private:
    virtual void fillHelp();
    virtual void initProperties();

private:
    QLabel *_destinationTitle;
    QLabel *_selectedPath;
    QPushButton *_selectDirectoryButton;
    QPushButton *_exportButton;
};

#endif // SAVEPROPERTYTAB_H
