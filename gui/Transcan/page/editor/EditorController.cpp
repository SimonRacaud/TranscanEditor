#include "EditorController.h"
#include "../../window/MainWindow.h"
#include "widget/misc/Notification.h"
#include "utility/FileUtils.h"
#include "include/env_messages.h"

#include <functional>
#include <QMessageBox>

using namespace std;

extern MainWindow *mainWindow;

EditorController::EditorController(QWidget *parent) : EditorView(_api, parent), _maxTab(EditorTab::EXTRACT)
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

    // Base PropertyTab
    /// Zoom:
    connect(_extractPropTab, &APropertyTab::sigChangeZoom, this, &EditorController::setZoom);
    connect(_cleanPropTab, &APropertyTab::sigChangeZoom, this, &EditorController::setZoom);
    connect(_editPropTab, &APropertyTab::sigChangeZoom, this, &EditorController::setZoom);
    connect(_savePropTab, &APropertyTab::sigChangeZoom, this, &EditorController::setZoom);
    // Network
    connect(&_api, &APIClient::sigNetError, this, &EditorController::networkError);
}

void EditorController::keyPressEvent(QKeyEvent *event)
{
    /// ZOOM in image viewers
    if ((event->key() == Qt::Key::Key_Plus
         && (event->modifiers() == Qt::ControlModifier || event->modifiers() == Qt::KeypadModifier))
            || event->key() == Qt::Key::Key_ZoomIn) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());
        float zoom = editor->getZoom();
        this->setZoom(zoom + ZOOM_SHIFT);
    } else if ((event->key() == Qt::Key::Key_Minus
                && (event->modifiers() == Qt::ControlModifier || event->modifiers() == Qt::KeypadModifier))
               || event->key() == Qt::Key::Key_ZoomOut) {
        auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());
        float zoom = editor->getZoom();
        this->setZoom(zoom - ZOOM_SHIFT);
    } else {
        EditorView::keyPressEvent(event);
    }
}

/** Public **/

void EditorController::setZoom(float value)
{
    auto *editor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());
    auto *propertyTab = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());

    propertyTab->setZoom(value);
    editor->setZoom(value);
    _sourcePages->setZoom(value);
}

void EditorController::networkError(QString const &message)
{
    std::cerr << "Network error : " << message.toStdString() << std::endl;
    Notification::Build(ERR_NET_CORE, this);

    auto *prop = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
    if (prop) {
        prop->onNetworkError();
    }
}

void EditorController::onStart(ProjectConfig const &config)
{
    // Save project configuration
    this->_config = std::make_shared<ProjectConfig>(config);
    this->_extractEditTab->setConfig(_config);
    this->_editEditTab->setConfig(_config);
    this->_saveEditTab->setConfig(_config);
    //
    try {
        this->setTab(EditorTab::EXTRACT); // Extraction step
    } catch (std::exception const &err) {
        throw err; // Relay to main window
    }

    this->showSourcePageTab(true);
    // Load source images
    vector<OCRPage> pages = this->_sourcePages->loadPagesFromPath(_config->srcPath);
    try {
        FileUtils::createProjectDirectory(_config);
    } catch (std::exception const &err) {
        qDebug("FileUtils::createProjectDirectory ERROR : %s", err.what());
        throw std::runtime_error("Unable to create project direcotry");
    }
    if (!FileUtils::initProjectSourceImages(pages, _config->destPath)) {
        throw std::runtime_error("Failed to init project sources");
    }
    this->_extractEditTab->setPages(pages); // Load source images
    // OCR API call
    this->_extractEditTab->loadAPI();
    //
    this->_editPropTab->updateProjectConfig(config);
    this->_savePropTab->setProjectDestinationPath(config.destPath);
}

void EditorController::setTab(EditorTab tab)
{
    try {
        if ((int)tab > (int)_maxTab + 1) {
            Notification::Build(INFO_CANT_SWITCH_TAB, this, Notification::INFO);
            return;
        } else if ((int)tab > (int)_maxTab) {
            _maxTab = tab;
        }
        /// Previous Property tab, Viewer tab
        auto *prevProp = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
        auto *prevEditor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        /// Tab change, data flow
        IEditTab *prevEditTab = dynamic_cast<IEditTab *>(prevEditor);
        prevEditTab->unload(); // Unload previous tab
        // Warning: important to unload table before getting the pages for the Edit tab.
        // ...since the renderImagePath attribute will get updated
        std::vector<OCRPage> pages = prevEditor->getPages(); // Get tab result

        /// Disconnect previous event flow
        disconnect(prevProp, &APropertyTab::nextStep, nullptr, nullptr);
        disconnect(prevEditor, &ImageViewer::horizontalScrollValueChanged, nullptr, nullptr);
        disconnect(prevEditor, &ImageViewer::verticalScrollValueChanged, nullptr, nullptr);
        disconnect(_sourcePages, &ImageViewer::horizontalScrollValueChanged, nullptr, nullptr);
        disconnect(_sourcePages, &ImageViewer::verticalScrollValueChanged, nullptr, nullptr);

        /// Select new tabs
        this->_stackEdit->setCurrentIndex((int)tab);
        this->_stackProp->setCurrentIndex((int)tab);

        /// New Property tab, New Viewer tab
        APropertyTab *newProp = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
        ImageViewer *newEditor = dynamic_cast<ImageViewer *>(_stackEdit->currentWidget());

        /// Connect new event flow
        connect(newProp, &APropertyTab::nextStep, [this, tab]() {
            EditorTab next = ((int)tab + 1) >= (int)EditorTab::LAST_VALUE
                ? EditorTab::EXTRACT : (EditorTab)((int)tab + 1);
            try {
                this->setTab(next);
            } catch (std::exception const& err) {
                qDebug() << "APropertyTab::nextStep emit : exception on setTab, " << err.what();
                Notification::Build(ERR_FATAL_SWITCH_TAB, this);
            }
        });
        connect(newEditor, &ImageViewer::horizontalScrollValueChanged, _sourcePages, &ImageViewer::setHorizontalScrollPosition);
        connect(newEditor, &ImageViewer::verticalScrollValueChanged, _sourcePages, &ImageViewer::setVerticalScrollPosition);
        connect(_sourcePages, &ImageViewer::horizontalScrollValueChanged, newEditor, &ImageViewer::setHorizontalScrollPosition);
        connect(_sourcePages, &ImageViewer::verticalScrollValueChanged, newEditor, &ImageViewer::setVerticalScrollPosition);

        /// Update editor header state
        this->_header->setSelectionTabHeader(tab);

        /// Load new tabs
        IEditTab *newEditTab = dynamic_cast<IEditTab *>(newEditor);
        newEditTab->load(pages); // Load new tab
        newEditor->setZoom(_sourcePages->getZoom()); // Update Tab zoom
        newProp->setZoom(_sourcePages->getZoom());
        _sourcePages->emitScrollPosition();
    } catch (std::exception const &err) {
        qDebug() << "EditorController::setTab exception : " << err.what();
        Notification::Build(ERR_FATAL_SWITCH_TAB, this);
    }
}

void EditorController::showSourcePageTab(bool enable)
{
    this->_sourcePages->setHidden(!enable);
    this->_header->setShowSourceButtonState(enable);
}
