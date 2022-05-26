#include "homepage.h"
#include "../../ui/ui_home_page.h"
#include <QFileDialog>
#include "../../window/mainwindow.h"
#include <memory.h>

extern MainWindow *mainWindow;

HomePage::HomePage(QWidget *parent)
    : QWidget{parent}, ui(new Ui::HomePage)
{
    ui->setupUi(this);
    // Previous Project List
    this->previousProjectListModel = new QStringListModel(this);
    this->previousProjectListModel->setStringList(this->previousProjectList);
    ui->projectListView->setModel(this->previousProjectListModel);
}

HomePage::~HomePage()
{
    delete ui;
}

/** Internal functions **/

void HomePage::openViewer(const QString &path)
{
    if (path.isEmpty() == false && mainWindow != nullptr)
    {
        mainWindow->showViewer(path);
    }
}

void HomePage::addItemPreviousProjectList(const QString &path)
{
    this->previousProjectList << path;
    if (this->previousProjectList.size() > HomePage::PREV_PROJECT_LIST_SIZE_LIMIT) {
        this->previousProjectList.pop_front();
    }
    this->previousProjectListModel->setStringList(this->previousProjectList);
}

/** SLOTS **/

// Open specific directory in viewer
void HomePage::on_chapterViewerOpenButton_clicked()
{
    const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Image folder"));

    this->openViewer(folderPath);
    if (!folderPath.isEmpty()) {
        this->addItemPreviousProjectList(folderPath);
    }
}

// Open an old project in viewer
void HomePage::on_projectListView_doubleClicked(const QModelIndex &index)
{
    const QString &path = this->previousProjectList.at(index.row());

    this->openViewer(path);
}

