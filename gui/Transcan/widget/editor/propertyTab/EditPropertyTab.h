#ifndef EDITPROPERTYTAB_H
#define EDITPROPERTYTAB_H

#include "widget/editor/propertyTab/APropertyTab.h"
#include "include/environment.h"

#include <QFontComboBox>
#include <QSpinBox>
#include <QColorDialog>
#include <QPushButton>

class EditPropertyTab : public APropertyTab
{
public:
    EditPropertyTab(QWidget *parent = nullptr);

    void showProperties();
    void hideProperties();

private:
    virtual void fillHelp();
    virtual void initProperties();

private:
    QWidget *_formContainer;

    QPushButton *_colorSelectButton;
    QFontComboBox *_fontSelector;
    QSpinBox *_fontSizeSelector;
    QSpinBox *_lineHeightSelector;
};

#endif // EDITPROPERTYTAB_H
