#include "EditorController.h"

EditorController::EditorController(QWidget *parent) : EditorView(parent)
{
    this->setupEvents();
}

/** INTERNAL FUNCTIONS **/

void EditorController::setupEvents()
{
    QObject::connect(_showSourceButton, &QPushButton::clicked, this, &EditorController::showSourceButtonClickedSlot);
    QObject::connect(_extractButton, &QPushButton::clicked, this, &EditorController::extractButtonClickedSlot);
    QObject::connect(_cleanButton, &QPushButton::clicked, this, &EditorController::cleanButtonClickedSlot);
    QObject::connect(_editButton, &QPushButton::clicked, this, &EditorController::editButtonClickedSlot);
    QObject::connect(_saveButton, &QPushButton::clicked, this, &EditorController::saveButtonClickedSlot);
    QObject::connect(_exitButton, &QPushButton::clicked, this, &EditorController::exitButtonClickedSlot);
}

/** SLOTS **/

void EditorController::showSourceButtonClickedSlot(bool checked)
{
    // TODO
}
void EditorController::extractButtonClickedSlot(bool checked)
{
    // TODO
}
void EditorController::cleanButtonClickedSlot(bool checked)
{
    // TODO
}
void EditorController::editButtonClickedSlot(bool checked)
{
    // TODO
}
void EditorController::saveButtonClickedSlot(bool checked)
{
    // TODO
}
void EditorController::exitButtonClickedSlot(bool checked)
{
    // TODO
}
