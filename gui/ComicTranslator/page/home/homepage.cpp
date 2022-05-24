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
}

HomePage::~HomePage()
{
    delete ui;
}

void HomePage::on_chapterViewerOpenButton_clicked()
{
    const QString folderPath = QFileDialog::getExistingDirectory(this, tr("Image folder"));

    if (folderPath.isEmpty() == false && mainWindow != nullptr)
    {
        mainWindow->showViewer(folderPath);
    }
}
