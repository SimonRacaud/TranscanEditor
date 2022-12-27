#ifndef CLEANPROPERTYTAB_H
#define CLEANPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"
#include <QPushButton>
#include <QLabel>
#include <QPixmap>

class CleanPropertyTab : public APropertyTab
{
    Q_OBJECT
public:
    CleanPropertyTab(FuncNetCall &reloadFunc, QWidget *parent = nullptr);

signals:
    void sigReplacePage(QString const &filePath);

private:
    virtual void fillHelp();
    virtual void initProperties();

    void onFileSelectButtonClicked();

private:
    QPushButton *_fileSelect;
    QLabel *_fileSelectLabel;
};

#endif // CLEANPROPERTYTAB_H
