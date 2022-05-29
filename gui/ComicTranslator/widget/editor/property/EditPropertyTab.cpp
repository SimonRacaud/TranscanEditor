#include "EditPropertyTab.h"
#include <QFormLayout>

EditPropertyTab::EditPropertyTab(QWidget *parent) : APropertyTab(parent)
{
    this->fillHelp();
    this->initProperties();
}

void EditPropertyTab::fillHelp()
{
    this->_help->setText(
                "Edit:\n"
                "\n"
                "Actions:\n"
                "- Edit generated translation\n"
                "- Edit test style\n"
                "\n"
                "Controls:\n"
                "- Click on a box to select it\n"
                "- Double click to edit its content\n"
                );
}

void EditPropertyTab::initProperties()
{
    QFormLayout *formLayout = new QFormLayout;
    this->_formContainer = new QWidget;
    //
    this->_colorSelectButton = new QPushButton("Select");
    this->_colorSelectButton->setFixedWidth(EDITOR_PROP_FORM_WIDTH);
    this->_fontSelector = new QFontComboBox;
    this->_fontSelector->setFixedWidth(EDITOR_PROP_FORM_WIDTH);
    this->_fontSizeSelector = new QSpinBox;
    this->_fontSizeSelector->setFixedWidth(EDITOR_PROP_FORM_WIDTH);
    this->_fontSizeSelector->setRange(1, EDITOR_PROP_MAX_FONT_SIZE);
    this->_fontSizeSelector->setSingleStep(1);
    this->_fontSizeSelector->setValue(EDITOR_PROP_DEF_FONT_SIZE);
    this->_lineHeightSelector = new QSpinBox;
    this->_lineHeightSelector->setFixedWidth(EDITOR_PROP_FORM_WIDTH);
    this->_lineHeightSelector->setRange(1, 999);
    this->_lineHeightSelector->setValue(EDITOR_PROP_DEF_LINE_HEIGHT);
    //
    formLayout->addRow(tr("Color:"), _colorSelectButton);
    formLayout->addRow(tr("Font:"), _fontSelector);
    formLayout->addRow(tr("Font size:"), _fontSizeSelector);
    formLayout->addRow("Line height:", _lineHeightSelector);
    //
    this->_propertiesLayout->addWidget(_formContainer);
    this->_formContainer->setLayout(formLayout);
    this->hideProperties();
}

void EditPropertyTab::showProperties()
{
    this->_formContainer->setVisible(true);
}

void EditPropertyTab::hideProperties()
{
    this->_formContainer->setVisible(false);
}
