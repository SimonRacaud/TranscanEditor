#include "EditorController.h"
#include "../../window/MainWindow.h"

#include <functional>
#include <QMessageBox>

using namespace std;

extern MainWindow *mainWindow;

EditorController::EditorController(QWidget *parent) : EditorView(_api, parent), _lastTab(EditorTab::EXTRACT)
{
    this->setupEvents();
}

EditorController::~EditorController()
{
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

    connect(_cleanEditTab, &NetEditTab::allAPIRequestsCompleted, _cleanPropTab, &APropertyTab::unlockReloadButton);
    connect(_editEditTab, &NetEditTab::allAPIRequestsCompleted, _editPropTab, &APropertyTab::unlockReloadButton);
    connect(_extractEditTab, &NetEditTab::allAPIRequestsCompleted, _extractPropTab, &APropertyTab::unlockReloadButton);
    connect(_saveEditTab, &NetEditTab::allAPIRequestsCompleted, _savePropTab, &APropertyTab::unlockReloadButton);

    connect(_cleanPropTab, &CleanPropertyTab::sigReplacePage, _cleanEditTab, &CleanEditArea::slotReplacePage);

    // Edit Tab : Text Style Update
    connect(_editPropTab, &EditPropertyTab::sigUpdateClusterStyle, _editEditTab, &EditorEditArea::updateSelectedClusterStyle);
    connect(_editPropTab, &EditPropertyTab::sigUpdateAllClusterStyle, _editEditTab, &EditorEditArea::updateAllClusterStyle);
    connect(_editEditTab, &EditorEditArea::sigFocusEditRect, _editPropTab, &EditPropertyTab::onFocusCluster);
    connect(_editEditTab, &EditorEditArea::sigUnfocusEditRect, _editPropTab, &EditPropertyTab::onUnfocusCluster);
    // Save Tab
    connect(_savePropTab, &SavePropertyTab::sigUpdateProjectDestinationPath, _saveEditTab, &SaveEditArea::onUpdateProjectDestinationPath);
    connect(_savePropTab, &SavePropertyTab::sigExport, _saveEditTab, &SaveEditArea::onExport);
}

void EditorController::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key::Key_Plus || event->key() == Qt::Key::Key_ZoomIn) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        float zoom = editor->getZoom();
        editor->setZoom(zoom + ZOOM_SHIFT);
        _sourcePages->setZoom(zoom + ZOOM_SHIFT);
    } else if (event->key() == Qt::Key::Key_Minus || event->key() == Qt::Key::Key_ZoomOut) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        float zoom = editor->getZoom();
        editor->setZoom(zoom - ZOOM_SHIFT);
        _sourcePages->setZoom(zoom - ZOOM_SHIFT);
    } else {
        EditorView::keyPressEvent(event);
    }
}

/** Public **/

void EditorController::onStart(ProjectConfig const &config)
{
    // Save project configuration
    this->_config = std::make_shared<ProjectConfig>(config);
    this->_extractEditTab->setConfig(_config);
    this->_editEditTab->setConfig(_config);
    this->_saveEditTab->setConfig(_config);
    //
    this->setTab(EditorTab::EXTRACT); // Extraction step
    this->showSourcePageTab(true);
    // Load source images
    this->_sourcePages->loadPagesFromPath(_config->srcPath);
    this->_extractEditTab->loadPagesFromPath(_config->srcPath);
    // OCR API call
    this->_extractEditTab->loadAPI();
    //
    this->_editPropTab->updateProjectConfig(config);
    this->_savePropTab->setProjectDestinationPath(config.destPath);
}

void EditorController::setTab(EditorTab tab)
{
    if ((int)tab > (int)_lastTab + 1) {
        QMessageBox::information(this, "Tab access", "Please, do not skip any step of the process.");
        return;
    } else if ((int)tab > (int)_lastTab) {
        _lastTab = tab;
    }
    auto *prop = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
    auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

    // Tab change, data flow
    IEditTab *prevEditTab = dynamic_cast<IEditTab *>(editor);
    std::vector<OCRPage> pages = editor->getPages();
    prevEditTab->unload(); // Unload previous tab

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
    ImageViewer *newEditor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());
    connect(prop, &APropertyTab::nextStep, [this, tab]() {
        EditorTab next = ((int)tab + 1) >= (int)EditorTab::LAST_VALUE
            ? EditorTab::EXTRACT : (EditorTab)((int)tab + 1);
        this->setTab(next);
    });
    this->setSelectionTabHeader();
    connect(newEditor, &ImageViewer::horizontalScrollValueChanged, _sourcePages, &ImageViewer::setHorizontalScrollPosition);
    connect(newEditor, &ImageViewer::verticalScrollValueChanged, _sourcePages, &ImageViewer::setVerticalScrollPosition);
    connect(_sourcePages, &ImageViewer::horizontalScrollValueChanged, newEditor, &ImageViewer::setHorizontalScrollPosition);
    connect(_sourcePages, &ImageViewer::verticalScrollValueChanged, newEditor, &ImageViewer::setVerticalScrollPosition);

    // Load new tab
    IEditTab *newEditTab = dynamic_cast<IEditTab *>(newEditor);
    newEditTab->load(pages); // Load new tab
    newEditor->setZoom(_sourcePages->getZoom()); // Update Tab zoom
    _sourcePages->emitScrollPosition();
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
