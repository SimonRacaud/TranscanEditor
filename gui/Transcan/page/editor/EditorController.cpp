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
    connect(_cleanEditTab, &NetEditTab::allAPIRequestsCompleted, _cleanPropTab, &APropertyTab::unlockReloadButton);
    connect(_editEditTab, &NetEditTab::allAPIRequestsCompleted, _editPropTab, &APropertyTab::unlockReloadButton);
    connect(_extractEditTab, &NetEditTab::allAPIRequestsCompleted, _extractPropTab, &APropertyTab::unlockReloadButton);
    connect(_saveEditTab, &NetEditTab::allAPIRequestsCompleted, _savePropTab, &APropertyTab::unlockReloadButton);

    connect(_cleanPropTab, &CleanPropertyTab::sigReplacePage, _cleanEditTab, &CleanEditTab::slotReplacePage);

    // Edit Tab : Text Style Update
    connect(_editPropTab, &EditPropertyTab::sigUpdateClusterStyle, _editEditTab, &EditorEditTab::updateSelectedClusterStyle);
    connect(_editPropTab, &EditPropertyTab::sigUpdateAllClusterStyle, _editEditTab, &EditorEditTab::updateAllClusterStyle);
    connect(_editEditTab, &EditorEditTab::sigFocusEditRect, _editPropTab, &EditPropertyTab::onFocusCluster);
    connect(_editEditTab, &EditorEditTab::sigUnfocusEditRect, _editPropTab, &EditPropertyTab::onUnfocusCluster);
    // Save Tab
    connect(_savePropTab, &SavePropertyTab::sigUpdateProjectDestinationPath, _saveEditTab, &SaveEditTab::onUpdateProjectDestinationPath);
    connect(_savePropTab, &SavePropertyTab::sigExport, _saveEditTab, &SaveEditTab::onExport);

    // Header
    connect(_header, &EditorHeader::sigChangeTab, this, &EditorController::setTab);
    connect(_header, &EditorHeader::sigShowPageSourceTab, this, &EditorController::showSourcePageTab);

    connect(this, &EditorView::sigChangeTab, this, &EditorController::setTab);
}

void EditorController::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key::Key_Plus
         && (event->modifiers() == Qt::ControlModifier || event->modifiers() == Qt::KeypadModifier))
            || event->key() == Qt::Key::Key_ZoomIn) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        float zoom = editor->getZoom();
        editor->setZoom(zoom + ZOOM_SHIFT);
        _sourcePages->setZoom(zoom + ZOOM_SHIFT);
    } else if ((event->key() == Qt::Key::Key_Minus
                && (event->modifiers() == Qt::ControlModifier || event->modifiers() == Qt::KeypadModifier))
               || event->key() == Qt::Key::Key_ZoomOut) {
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
    prevEditTab->unload(); // Unload previous tab
    // Nota: important to unload table before getting the pages for the Edit tab.
    // ...since the renderImagePath attribute will get updated
    std::vector<OCRPage> pages = editor->getPages();

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
    this->_header->setSelectionTabHeader(tab);
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
    this->_header->setShowSourceButtonState(enable);
}
