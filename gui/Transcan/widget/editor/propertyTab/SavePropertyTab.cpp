#include "SavePropertyTab.h"
#include <QVBoxLayout>
#include "window/MainWindow.h"

extern MainWindow *mainWindow;

SavePropertyTab::SavePropertyTab(FuncNetCall &reloadFunc, QWidget *parent)
    : APropertyTab(reloadFunc, parent)
{
    this->fillHelp();
    this->initProperties();
    this->_nextButton->setEnabled(false);
    this->_reloadButton->hide();

    disconnect(_nextButton, &QPushButton::clicked, nullptr, nullptr);
    connect(_nextButton, &QPushButton::clicked, []() {
       mainWindow->setPage(Page::HOME); // Go back home
    });
    connect(_selectDirectoryButton, &QPushButton::clicked, this, &SavePropertyTab::onClickSelectDestinationPath);
    connect(_exportButton, &QPushButton::clicked, [this]() {
        emit this->sigExport();
    });
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
    this->_destinationTitle->setObjectName("PropertyTitle");
    this->_destinationTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    this->_selectedPath = new QLabel("<>");
    this->_selectedPath->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    _selectedPath->setObjectName("SavePropTabDestPath");
    _selectedPath->setWordWrap(true);
    this->_selectedPath->setMaximumWidth(EDITOR_PROP_FORM_WIDTH);
    this->_selectDirectoryButton = new QPushButton("Change directory");
    destSelectLayout->addWidget(_destinationTitle);
    destSelectLayout->addWidget(_selectedPath);
    destSelectLayout->addWidget(_selectDirectoryButton);
    destSelectLayout->setSpacing(10);
    //
    this->_exportButton = new QPushButton("Export again");
    this->_exportButton->setEnabled(false);
    destSelectLayout->addWidget(_exportButton);

    this->_propertiesLayout->addLayout(destSelectLayout);
    this->_propertiesLayout->setSpacing(50);
}

/* SLOTS */

void SavePropertyTab::setProjectDestinationPath(QString const &path)
{
    this->_selectedPath->setText(path);
}

void SavePropertyTab::onClickSelectDestinationPath()
{
    const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Destination image folder"));

    if (!folderPath.isEmpty()) {
        emit this->sigUpdateProjectDestinationPath(folderPath);
        this->setProjectDestinationPath(folderPath);
        this->_exportButton->setEnabled(true);
    }
}
