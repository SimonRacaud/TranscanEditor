#ifndef EDITORVIEW_H
#define EDITORVIEW_H

#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QStackedWidget>

#include "widget/imageViewer/ImageViewer.h"

#include "widget/editor/propertyTab/CleanPropertyTab.h"
#include "widget/editor/propertyTab/EditPropertyTab.h"
#include "widget/editor/propertyTab/ExtractionPropertyTab.h"
#include "widget/editor/propertyTab/SavePropertyTab.h"

#include "widget/editor/editTab/ExtractionEditTab.h"
#include "widget/editor/editTab/CleanEditTab.h"
#include "widget/editor/editTab/EditorEditTab.h"
#include "widget/editor/editTab/SaveEditTab.h"

#include "include/environment.h"
#include "widget/editor/header/EditorHeader.h"

/**
 * @brief The EditorView class : Editor page's view
 */
class EditorView : public QWidget
{
    Q_OBJECT
public:
    explicit EditorView(APIClient &api, QWidget *parent = nullptr);
protected:

signals:
    void sigChangeTab(EditorTab tab);

private:
    void setupHeader();

protected:
    QVBoxLayout *_rootLayout;
    EditorHeader *_header;

    // Body
    //      Property tabs
    CleanPropertyTab *_cleanPropTab;
    EditPropertyTab *_editPropTab;
    ExtractionPropertyTab *_extractPropTab;
    SavePropertyTab *_savePropTab;

    //      Editors
    ExtractionEditTab *_extractEditTab;
    CleanEditTab *_cleanEditTab;
    EditorEditTab *_editEditTab;
    SaveEditTab *_saveEditTab;
    //      Stacks
    QStackedWidget *_stackEdit;
    QStackedWidget *_stackProp;
    ImageViewer *_sourcePages;
};

#endif // EDITORVIEW_H
