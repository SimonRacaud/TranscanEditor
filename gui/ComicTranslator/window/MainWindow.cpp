#include "MainWindow.h"
#include <QGridLayout>

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _stack(nullptr), _homePage(nullptr), _viewer(nullptr)
{
    this->_stack = new QStackedWidget;
    this->_homePage = new HomePage;
    this->_editor = new EditorController;
    this->_stack->addWidget(this->_homePage);
    this->_stack->addWidget(this->_editor);
    this->setCentralWidget(this->_stack);
    this->centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    this->setPage(Page::EDITOR); // DEBUG temp

    // Set background color
    QPalette pal = QPalette(QColor(0xFF0000));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
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

void MainWindow::setPage(Page page)
{
    switch (page) {
        case Page::HOME:
            this->_stack->setCurrentIndex(0);
        break;
        case Page::EDITOR:
            this->_stack->setCurrentIndex(1);
        break;
    default:
        throw std::runtime_error("MainWindow::setPage page unsupported");
    }
}
