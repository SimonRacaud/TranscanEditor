#include "CleanPropertyTab.h"
#include <QHBoxLayout>

CleanPropertyTab::CleanPropertyTab(QWidget *parent) : APropertyTab(parent)
{
    this->fillHelp();
    this->initProperties();
}

void CleanPropertyTab::fillHelp()
{
    this->_help->setText(
                "Clean:\n"
                "\n"
                "This software only allow very easy cleaning but "
                "you can import cleaned page from another "
                "software.\n"
                "\n"
                "Actions:\n"
                "- Enable and disable clean boxes.\n"
                "\n"
                "Controls:\n"
                "- Click on a box to change it state\n"
                "- Click on a page to select it\n"
                );
}

void CleanPropertyTab::initProperties()
{
    QHBoxLayout *inputLayout = new QHBoxLayout;
    this->_fileSelect = new QPushButton("Select file");
    this->_fileSelectLabel = new QLabel("Replace page: ");
    inputLayout->addWidget(_fileSelectLabel);
    inputLayout->addWidget(_fileSelect);
    inputLayout->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    //
    //this->_previewImg.load();
    this->_previewWidget = new QLabel();
    this->_previewWidget->setPixmap(_previewImg);
    this->_previewWidget->setFixedHeight(300);
    this->_previewWidget->setStyleSheet("background-color: #EEEEEE");
    this->_applyButton = new QPushButton("Apply");
    //
    this->_propertiesLayout->addLayout(inputLayout);
    this->_propertiesLayout->addWidget(_previewWidget);
    this->_propertiesLayout->addWidget(_applyButton);
}
