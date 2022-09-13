#include "EditorController.h"
#include "../../window/MainWindow.h"

extern MainWindow *mainWindow;

EditorController::EditorController(QWidget *parent) : EditorView(parent)
{
    this->setupEvents();
    this->setTab(EditorTab::EXTRACT);
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

/** Public **/

void EditorController::setTab(EditorTab tab)
{
    auto *prop = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
    disconnect(prop, &APropertyTab::nextStep, nullptr, nullptr);

    this->_stackEdit->setCurrentIndex((int)tab);
    this->_stackProp->setCurrentIndex((int)tab);

    prop = dynamic_cast<APropertyTab *>(_stackProp->currentWidget());
    connect(prop, &APropertyTab::nextStep, [this, tab]() {
        EditorTab next = ((int)tab + 1) > (int)EditorTab::SAVE
            ? EditorTab::EXTRACT : (EditorTab)((int)tab + 1);
        this->setTab(next);
    });
    this->setSelectionTabHeader();
}

/** SLOTS **/

void EditorController::showSourceButtonClickedSlot(bool checked)
{
    // show/hide source document tab
    AEditArea *widget = dynamic_cast<AEditArea *>(_stackEdit->currentWidget());
    widget->showSourceView(checked);
}
void EditorController::extractButtonClickedSlot(bool checked)
{
    this->setTab(EditorTab::EXTRACT);
}
void EditorController::cleanButtonClickedSlot(bool checked)
{
    this->setTab(EditorTab::CLEAN);
}
void EditorController::editButtonClickedSlot(bool checked)
{
    this->setTab(EditorTab::EDIT);
}
void EditorController::saveButtonClickedSlot(bool checked)
{
    this->setTab(EditorTab::SAVE);
}
void EditorController::exitButtonClickedSlot(bool checked)
{
    // Go back to home page
    mainWindow->setPage(Page::HOME);
}
