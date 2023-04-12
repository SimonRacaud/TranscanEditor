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

#include "widget/editor/editTab/ExtractionEditArea.h"
#include "widget/editor/editTab/CleanEditArea.h"
#include "widget/editor/editTab/EditorEditArea.h"
#include "widget/editor/editTab/SaveEditArea.h"

#include "include/environment.h"
#include "widget/editor/header/EditorHeader.h"

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
