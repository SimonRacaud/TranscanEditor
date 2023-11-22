#include "CleanPropertyTab.h"

#include "include/env_config.h"
#include <QHBoxLayout>
#include <QFileDialog>

CleanPropertyTab::CleanPropertyTab(FuncNetCall &reloadFunc, QWidget *parent)
    : APropertyTab(reloadFunc, parent)
{
    this->fillHelp();
    this->initProperties();
}

/**
 * PRIVATE
 */

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
    this->_fileSelectLabel = new QLabel("Replace current page: ");
    inputLayout->addWidget(_fileSelectLabel);
    inputLayout->addWidget(_fileSelect);
    inputLayout->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    //
    this->_propertiesLayout->addLayout(inputLayout);
    connect(_fileSelect, &QPushButton::clicked, this, &CleanPropertyTab::onFileSelectButtonClicked);
}

void CleanPropertyTab::onFileSelectButtonClicked()
{
    const QString &filePath = QFileDialog::getOpenFileName(this, tr("Select Image"), "", tr(IMG_SELECT_FILTER));

    if (!filePath.isEmpty()) {
        emit this->sigReplacePage(filePath);
    }
}
