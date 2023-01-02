#ifndef SAVEPROPERTYTAB_H
#define SAVEPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"
#include "include/environment.h"

#include <QLabel>
#include <QPushButton>

class SavePropertyTab : public APropertyTab
{
    Q_OBJECT
public:
    SavePropertyTab(FuncNetCall &reloadFunc, QWidget *parent = nullptr);

private:
    void fillHelp();
    void initProperties();

signals:
    void sigUpdateProjectDestinationPath(QString const &path);
    void sigExport();

public slots:
    void setProjectDestinationPath(QString const &path);

private slots:
    void onClickSelectDestinationPath();

private:
    QLabel *_destinationTitle;
    QLabel *_selectedPath;
    QPushButton *_selectDirectoryButton;
    QPushButton *_exportButton;
};

#endif // SAVEPROPERTYTAB_H
