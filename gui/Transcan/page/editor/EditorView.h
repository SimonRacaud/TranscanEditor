#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QtSvgWidgets/QSvgWidget>

#include "widget/imageViewer/ImageViewer.h"

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
    explicit EditorView(APIClient &api, QWidget *parent = nullptr);

    enum class EditorTab {
        EXTRACT = 0,
        CLEAN = 1,
        EDIT = 2,
        SAVE = 3,
        LAST_VALUE // Nothing after this
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
    QSvgWidget *_separator1;
    QSvgWidget *_separator2;
    QSvgWidget *_separator3;

    // Body
    //      Property tabs
    CleanPropertyTab *_cleanPropTab;
    EditPropertyTab *_editPropTab;
    ExtractionPropertyTab *_extractPropTab;
    SavePropertyTab *_savePropTab;

    //      Editors
    ExtractionEditArea *_extractEditTab;
    CleanEditArea *_cleanEditTab;
    EditorEditArea *_editEditTab;
    SaveEditArea *_saveEditTab;
    //      Stacks
    QStackedWidget *_stackEdit;
    QStackedWidget *_stackProp;
    ImageViewer *_sourcePages;
};

#endif // EDITORVIEW_H
