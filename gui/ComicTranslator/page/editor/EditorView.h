#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>

#include "widget/editor/editTab/AEditArea.h"
#include "widget/editor/propertyTab/APropertyTab.h"
#include "include/environment.h"

class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(QWidget *parent = nullptr);

private:
    void setupHeader();

protected:
    QVBoxLayout *_rootLayout;
    // Header
    QPushButton *_showSourceButton;
    QPushButton *_extractButton;
    QPushButton *_cleanButton;
    QPushButton *_editButton;
    QPushButton *_saveButton;
    QPushButton *_exitButton;
    QLabel *_separator1;
    QLabel *_separator2;
    QLabel *_separator3;

    AEditArea *_editArea;
    APropertyTab *_propertyTab;
};

#endif // EDITORVIEW_H
