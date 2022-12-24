#include "SavePropertyTab.h"
#include <QVBoxLayout>

SavePropertyTab::SavePropertyTab(FuncNetCall &reloadFunc, QWidget *parent)
    : APropertyTab(reloadFunc, parent)
{
    this->fillHelp();
    this->initProperties();
    this->_nextButton->setText("Home Page");
    this->_reloadButton->setText("Render again");
}

void SavePropertyTab::fillHelp()
{
    this->_help->setText(
                "Save:\n"
                "\n"
                "Congratulation ! The translation is finished !\n"
                "\n"
                "You can now export the pages into the "
                "destination directory.\n"
                "\n"
                "Vous pouvez retounez aux étapes précédentes "
                "via la bar de séléction dans le header.\n"
                );
}

void SavePropertyTab::initProperties()
{
    QVBoxLayout *destSelectLayout = new QVBoxLayout;
    this->_destinationTitle = new QLabel("Destination:");
    this->_destinationTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    this->_selectedPath = new QLabel("/path/test");
    this->_selectedPath->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    this->_selectDirectoryButton = new QPushButton("Select directory");
    destSelectLayout->addWidget(_destinationTitle);
    destSelectLayout->addWidget(_selectedPath);
    destSelectLayout->addWidget(_selectDirectoryButton);
    destSelectLayout->setSpacing(10);
    //
    this->_exportButton = new QPushButton("Export");

    this->_propertiesLayout->addLayout(destSelectLayout);
    this->_propertiesLayout->addWidget(_exportButton);
    this->_propertiesLayout->setSpacing(50);
}
