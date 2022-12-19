#include "EditorController.h"
#include "../../window/MainWindow.h"

#include <functional>
#include <QMessageBox>

using namespace std;

extern MainWindow *mainWindow;

EditorController::EditorController(QWidget *parent) : EditorView(parent)
{
    this->setupEvents();

//    vector<OCRPage> debug = { // TODO: debug
//        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
//        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
//        { .imagePath = "/media/work/personnal-projects/scanTranslator/scrapper/data_overlord/Chapter_1/5-o.jpg" },
//    };
//    this->_sourcePages->setPages(debug); // DEBUG
}

EditorController::~EditorController()
{
    if (_config != nullptr)
        delete _config;
}

/** INTERNAL FUNCTIONS **/

void EditorController::setupEvents()
{
    QObject::connect(_showSourceButton, &QPushButton::toggled, this, &EditorController::showSourceButtonClickedSlot);
    QObject::connect(_extractButton, &QPushButton::clicked, this, &EditorController::extractButtonClickedSlot);
    QObject::connect(_cleanButton, &QPushButton::clicked, this, &EditorController::cleanButtonClickedSlot);
    QObject::connect(_editButton, &QPushButton::clicked, this, &EditorController::editButtonClickedSlot);
    QObject::connect(_saveButton, &QPushButton::clicked, this, &EditorController::saveButtonClickedSlot);
    QObject::connect(_exitButton, &QPushButton::clicked, this, &EditorController::exitButtonClickedSlot);
}

void EditorController::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Plus || event->key() == Qt::Key::Key_ZoomIn) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        editor->scale(1.0 + ZOOM_SHIFT);
        _sourcePages->scale(1.0 + ZOOM_SHIFT);
    } else if (event->key() == Qt::Key::Key_Minus || event->key() == Qt::Key::Key_ZoomOut) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        editor->scale(1.0 - ZOOM_SHIFT);
        _sourcePages->scale(1.0 - ZOOM_SHIFT);
    } else {
        EditorView::keyPressEvent(event);
    }
}

/** Public **/

void EditorController::onStart(ProjectConfig const &config)
{
    // Save project configuration
    if (_config)
        delete _config;
    this->_config = new ProjectConfig(config);
    //
    this->setTab(EditorTab::EXTRACT); // Extraction step
    this->showSourcePageTab(true);
    // Load source images
    this->_sourcePages->loadPagesFromPath(_config->srcPath);
    this->_extractEditTab->loadPagesFromPath(_config->srcPath);
    // OCR API call
    std::vector<OCRPage> const &pages = this->_extractEditTab->getPages();
    size_t i = 0;
    for (OCRPage const &page : pages) {
        using std::placeholders::_1;
        NetCallback callback = bind(&IEditTab::loadPage, _extractEditTab, _1);
        NetErrCallback errCallback = bind(&EditorController::netError, this, _1);
        this->_api.sendToOCR(config, i, page.imagePath, callback, errCallback);
        i++;
    }
}

void EditorController::netError(QString const &message)
{
    ImageViewer *widget = static_cast<ImageViewer *>(this->_stackEdit->currentWidget());
    widget->setLoadingState(false);
    QMessageBox::information(this, "Network error", message);
}

void EditorController::setTab(EditorTab tab)
{
    auto *prop = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
    auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

    // Disconnect previous event flow
    disconnect(prop, &APropertyTab::nextStep, nullptr, nullptr);
    disconnect(editor, &ImageViewer::horizontalScrollValueChanged, nullptr, nullptr);
    disconnect(editor, &ImageViewer::verticalScrollValueChanged, nullptr, nullptr);
    disconnect(_sourcePages, &ImageViewer::horizontalScrollValueChanged, nullptr, nullptr);
    disconnect(_sourcePages, &ImageViewer::verticalScrollValueChanged, nullptr, nullptr);

    // Select new tabs
    this->_stackEdit->setCurrentIndex((int)tab);
    this->_stackProp->setCurrentIndex((int)tab);

    // Connect new event flow
    prop = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
    editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());
    connect(prop, &APropertyTab::nextStep, [this, tab]() {
        EditorTab next = ((int)tab + 1) > (int)EditorTab::SAVE
            ? EditorTab::EXTRACT : (EditorTab)((int)tab + 1);
        this->setTab(next);
    });
    this->setSelectionTabHeader();
    connect(editor, &ImageViewer::horizontalScrollValueChanged, _sourcePages, &ImageViewer::setHorizontalScrollPosition);
    connect(editor, &ImageViewer::verticalScrollValueChanged, _sourcePages, &ImageViewer::setVerticalScrollPosition);
    connect(_sourcePages, &ImageViewer::horizontalScrollValueChanged, editor, &ImageViewer::setHorizontalScrollPosition);
    connect(_sourcePages, &ImageViewer::verticalScrollValueChanged, editor, &ImageViewer::setVerticalScrollPosition);
}

void EditorController::showSourcePageTab(bool enable)
{
    this->_sourcePages->setHidden(!enable);
    this->_showSourceButton->setChecked(enable);
}

/** SLOTS **/

void EditorController::showSourceButtonClickedSlot(bool checked)
{
    // show/hide source document tab
    this->showSourcePageTab(checked);
}
void EditorController::extractButtonClickedSlot(bool)
{
    this->setTab(EditorTab::EXTRACT);
}
void EditorController::cleanButtonClickedSlot(bool)
{
    this->setTab(EditorTab::CLEAN);
}
void EditorController::editButtonClickedSlot(bool)
{
    this->setTab(EditorTab::EDIT);
}
void EditorController::saveButtonClickedSlot(bool)
{
    this->setTab(EditorTab::SAVE);
}
void EditorController::exitButtonClickedSlot(bool)
{
    // Go back to home page
    mainWindow->setPage(Page::HOME);
}
