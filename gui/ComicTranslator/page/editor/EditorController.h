#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <QWidget>
#include "EditorView.h"

class EditorController : public EditorView
{
    Q_OBJECT
public:
    EditorController(QWidget *parent = nullptr);

private:
    void setupEvents();

private slots:
    void showSourceButtonClickedSlot(bool checked);
    void extractButtonClickedSlot(bool checked);
    void cleanButtonClickedSlot(bool checked);
    void editButtonClickedSlot(bool checked);
    void saveButtonClickedSlot(bool checked);
    void exitButtonClickedSlot(bool checked);

};

#endif // EDITORCONTROLLER_H