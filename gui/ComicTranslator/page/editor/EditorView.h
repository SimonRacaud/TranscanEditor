#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "widget/editor/propertyTab/CleanPropertyTab.h"
#include "widget/editor/propertyTab/EditPropertyTab.h"
#include "widget/editor/propertyTab/ExtractionPropertyTab.h"
#include "widget/editor/propertyTab/SavePropertyTab.h"

#include "widget/editor/editTab/ExtractionEditArea.h"
#include "widget/editor/editTab/CleanEditArea.h"
#include "widget/editor/editTab/EditorEditArea.h"
#include "widget/editor/editTab/SaveEditArea.h"

#include "include/environment.h"

class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(QWidget *parent = nullptr);

    enum class EditorTab {
        EXTRACT = 0,
        CLEAN = 1,
        EDIT = 2,
        SAVE = 3
    };

protected:
    /**
     * @brief Disable current tab button
     */
    void setSelectionTabHeader();

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

    CleanPropertyTab *_cleanPropTab;
    EditPropertyTab *_editPropTab;
    ExtractionPropertyTab *_extractPropTab;
    SavePropertyTab *_savePropTab;

    ExtractionEditArea *_extractEditTab;
    CleanEditArea *_cleanEditTab;
    EditorEditArea *_editEditTab;
    SaveEditArea *_saveEditTab;

    QStackedWidget *_stackEdit;
    QStackedWidget *_stackProp;
};

#endif // EDITORVIEW_H
