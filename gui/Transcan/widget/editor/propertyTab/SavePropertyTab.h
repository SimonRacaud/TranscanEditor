#ifndef SAVEPROPERTYTAB_H
#define SAVEPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"
#include "include/environment.h"

#include <QLabel>
#include <QPushButton>

class SavePropertyTab : public APropertyTab
{
public:
    SavePropertyTab(FuncNetCall &reloadFunc, QWidget *parent = nullptr);

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
