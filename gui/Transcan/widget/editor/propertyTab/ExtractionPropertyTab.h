#ifndef EXTRACTIONPROPERTYTAB_H
#define EXTRACTIONPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"

class ExtractionPropertyTab : public APropertyTab
{
public:
    ExtractionPropertyTab(QWidget *parent = nullptr);

private:
    virtual void fillHelp();
    virtual void initProperties();
};

#endif // EXTRACTIONPROPERTYTAB_H
