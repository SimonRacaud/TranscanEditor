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
    // Create project form
    this->loadNewProjectFormContent();
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

void HomePage::loadNewProjectFormContent()
{
    this->ui->sourceSelectLabel->setVisible(false);
    this->ui->destSelectLabel->setVisible(false);
    this->ui->ocrComboBox->addItems(OCR_SERVICE_LIST);
    this->ui->ocrWarningLabel->setVisible(false);
    this->ui->transComboBox->addItems(TRANS_SERVICE_LIST);
    this->ui->transWarningLabel->setVisible(false);
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

// Project create form : Choose source directory
void HomePage::on_srcSelectButton_clicked()
{
    const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Source image folder"));

    if (!folderPath.isEmpty()) {
        this->sourceDirectory = folderPath;
        this->ui->sourceSelectLabel->setText(folderPath);
        this->ui->sourceSelectLabel->setVisible(true);
    }
}

// Project create form : Choose destination directory
void HomePage::on_destButton_clicked()
{
    const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Destination image folder"));

    if (!folderPath.isEmpty()) {
        this->destDirectory = folderPath;
        this->ui->destSelectLabel->setText(folderPath);
        this->ui->destSelectLabel->setVisible(true);
    }
}

// Project create form : Apply project creation
void HomePage::on_submitButton_clicked()
{

}

