#include "MainWindow.h"
#include <QGridLayout>
#include "include/models.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), _stack(nullptr), _homePage(nullptr), _viewer(nullptr)
{
    this->_stack = new QStackedWidget;
    this->_homePage = new HomePage;
    this->_stack->addWidget(this->_homePage);
    this->setCentralWidget(this->_stack);
    this->centralWidget()->layout()->setContentsMargins(0, 0, 0, 0);

    this->setPage(Page::HOME);

    // Set background color
    QPalette pal = QPalette(QColor(0xFFFFFF));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

     this->resize(EDITOR_DEF_SIZE);
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
    this->_editor->onStart(config);
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
