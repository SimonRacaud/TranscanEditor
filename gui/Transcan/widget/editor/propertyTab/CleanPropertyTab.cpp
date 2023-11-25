#include "CleanPropertyTab.h"

#include "include/env_config.h"
#include "include/env_messages.h"
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
    this->_help->setText(CLEAN_HELP);
}

void CleanPropertyTab::initProperties()
{
    QVBoxLayout *inputLayout = new QVBoxLayout;
    this->_fileSelect = new QPushButton(TXT_SELECT_FILE);
    this->_fileSelectLabel = new QLabel(TXT_REPLACE_PAGE);
    inputLayout->addWidget(_fileSelectLabel);
    inputLayout->addWidget(_fileSelect);
    //
    this->_propertiesLayout->addLayout(inputLayout);
    connect(_fileSelect, &QPushButton::clicked, this, &CleanPropertyTab::onFileSelectButtonClicked);
}

void CleanPropertyTab::onFileSelectButtonClicked()
{
    const QString &filePath = QFileDialog::getOpenFileName(this, TXT_SELECT_IMG, "", IMG_SELECT_FILTER);

    if (!filePath.isEmpty()) {
        emit this->sigReplacePage(filePath);
    }
}
