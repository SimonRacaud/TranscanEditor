#include "MainWindow.h"
#include <QGridLayout>
#include <QMessageBox>
#include "model/configModels.h"
#include "widget/misc/Notification.h"
#include "include/env_style.h"
#include "include/env_messages.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _stack(nullptr), _homePage(nullptr), _viewer(nullptr)
{
    this->_stack = new QStackedWidget;
    this->_homePage = new HomePage;
    this->_stack->addWidget(this->_homePage);
    this->setCentralWidget(this->_stack);
    this->centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    this->setPage(Page::HOME);

    //this->resize(EDITOR_DEF_SIZE);
    this->showMaximized();
}

MainWindow::~MainWindow()
{
    delete this->_viewer;
}

void MainWindow::showViewer(QString const &dirPath)
{
    if (this->_viewer == nullptr)
    {
        this->_viewer = new ViewerWindow;
    }
    this->_viewer->show();
    this->_viewer->loadDirectory(dirPath);
}

void MainWindow::setConfig(ProjectConfig const &config)
{
    try {
        this->_editor->onStart(config);
    } catch (std::exception const &err) {
        qDebug("MainWindow::setConfig exception : %s\n", err.what());
        Notification::Build(ERR_FATAL_LOAD_PROJECT, this);
        this->setPage(Page::HOME); // Go back home
    }
}

void MainWindow::setPage(Page page)
{
    switch (page) {
        case Page::HOME:
            this->_stack->setCurrentWidget(_homePage);
        break;
        case Page::EDITOR:
            if (_editor) { // remove previous instance
                this->_stack->removeWidget(_editor);
                delete _editor;
            }
            this->_editor = new EditorController(this);
            this->_stack->addWidget(this->_editor);

            this->_stack->setCurrentWidget(_editor);
        break;
    default:
        throw std::runtime_error("MainWindow::setPage page unsupported");
    }
}
