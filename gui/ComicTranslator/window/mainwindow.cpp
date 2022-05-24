#include "mainwindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _stack(nullptr), _homePage(nullptr), _viewer(nullptr)
{
    this->_stack = new QStackedWidget;
    this->_homePage = new HomePage;
    this->_stack->addWidget(this->_homePage);

    setCentralWidget(this->_stack);
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
