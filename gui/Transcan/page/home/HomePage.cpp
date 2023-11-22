#include "HomePage.h"
#include "ui_home_page.h"

#include "window/MainWindow.h"
#include "include/env_services.h"

#include <QFileDialog>
#include <QMessageBox>

extern MainWindow *mainWindow;

HomePage::HomePage(QWidget *parent)
    : QWidget{parent}, ui(new Ui::HomePage)
{
    ui->setupUi(this);
    _fontSelector = new FontSelect;
    ui->fontSelectLayout->addWidget(_fontSelector);
    // Previous Project List
    this->previousProjectListModel = new QStringListModel(this);
    this->previousProjectListModel->setStringList(this->previousProjectList);
    ui->projectListView->setModel(this->previousProjectListModel);
    // Create project form
    this->loadNewProjectFormContent();

    // TODO : DEBUG auto fill form
    this->ui->projectName->setText("TestProject");
    this->sourceDirectory = "/home/simon/scanTranslator/data/dataset/debug";
    this->destDirectory = "/home/simon/scanTranslator/data/result";
    this->ui->ocrComboBox->setCurrentIndex(1); // AWS
    this->ui->transComboBox->setCurrentIndex(1); // GOOGLE
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
    if ((size_t)this->previousProjectList.size() > HomePage::PREV_PROJECT_LIST_SIZE_LIMIT) {
        this->previousProjectList.pop_front();
    }
    this->previousProjectListModel->setStringList(this->previousProjectList);
}

void HomePage::loadNewProjectFormContent()
{
    this->ui->sourceSelectLabel->setVisible(false);
    this->ui->destSelectLabel->setVisible(false);
    for (auto &service : OCR_SERVICE_LIST) {
        this->ui->ocrComboBox->addItem(service.name);
    }
    this->ui->ocrWarningLabel->setVisible(false);
    for (auto &service : TRANS_SERVICE_LIST) {
        this->ui->transComboBox->addItem(service.name);
    }
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
    const QString &srcDirectory = this->sourceDirectory;
    const QString &destDirectory = this->destDirectory;
    const int OCRServiceIndex = this->ui->ocrComboBox->currentIndex();
    const int translationServiceIndex = this->ui->transComboBox->currentIndex();
    const QString &srcLang = (translationServiceIndex) ? this->ui->transSrcComboBox->currentText() : nullptr;
    const QString &destLang = (translationServiceIndex) ? this->ui->transDestComboBox->currentText() : nullptr;
    const QFont &font = _fontSelector->currentFont();
    const QString &projectName = this->ui->projectName->text();

    if (projectName.isEmpty()) {
        QMessageBox::information(this, tr("Missing information"), tr("Project name required"));
        return; // Abort
    }
    //
    ProjectConfig config(
                projectName,
                srcDirectory,
                destDirectory,
                OCR_SERVICE_LIST[OCRServiceIndex].apiLabel,
                TRANS_SERVICE_LIST[translationServiceIndex].apiLabel,
                srcLang,
                destLang,
                font);

    mainWindow->setPage(Page::EDITOR);
    mainWindow->setConfig(config);
}

// Change selection OCR Service
void HomePage::on_ocrComboBox_currentIndexChanged(int index)
{
    // Show or hide configuration requirement message
    if (index < (int)sizeof(OCR_SERVICE_LIST) && index >= 0
            && OCR_SERVICE_LIST[index].needConfig) {
        this->ui->ocrWarningLabel->setVisible(true);
        if (OCR_SERVICE_LIST[index].helpMessage.isEmpty() == false) {
            this->ui->ocrWarningLabel->setText(OCR_SERVICE_LIST[index].helpMessage);
        } else {
            this->ui->ocrWarningLabel->setText(SERVICE_DEFAULT_HELP);
        }
    } else {
        this->ui->ocrWarningLabel->setVisible(false);
    }
}

// Change selection Translation Service
void HomePage::on_transComboBox_currentIndexChanged(int index)
{
    // Show or hide configuration requirement message
    if (index < (int)sizeof(TRANS_SERVICE_LIST) && index >= 0) {
        if (TRANS_SERVICE_LIST[index].needConfig) {
            this->ui->transWarningLabel->setVisible(true);
            if (TRANS_SERVICE_LIST[index].helpMessage.isEmpty() == false) {
                this->ui->transWarningLabel->setText(TRANS_SERVICE_LIST[index].helpMessage);
            } else {
                this->ui->transWarningLabel->setText(SERVICE_DEFAULT_HELP);
            }
        } else {
            this->ui->transWarningLabel->setVisible(false);
        }
        // Enable disable language selection
        if (this->ui->transComboBox->itemText(index) == TRANSLATE_DISABLED) {
            this->ui->transDestComboBox->setEnabled(false);
            this->ui->transSrcComboBox->setEnabled(false);
        } else {
            this->ui->transDestComboBox->setEnabled(true);
            this->ui->transSrcComboBox->setEnabled(true);
            this->ui->transDestComboBox->clear();
            this->ui->transSrcComboBox->clear();
            this->ui->transDestComboBox->addItems(TRANS_SERVICE_LIST[index].destLang);
            this->ui->transSrcComboBox->addItems(TRANS_SERVICE_LIST[index].srcLang);
        }
    }
}

