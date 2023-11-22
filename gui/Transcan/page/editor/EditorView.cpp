#include "EditorView.h"
#include "widget/editor/propertyTab/ExtractionPropertyTab.h"
#include "widget/editor/propertyTab/CleanPropertyTab.h"
#include "widget/editor/propertyTab/EditPropertyTab.h"
#include "widget/editor/propertyTab/SavePropertyTab.h"
#include "widget/editor/editTab/ExtractionEditTab.h"
#include "include/env_style.h"

#include <QHBoxLayout>
#include <QtSvgWidgets/QSvgWidget>

using std::bind;

EditorView::EditorView(APIClient &api, QWidget *parent)
    : QWidget{parent}, _rootLayout(nullptr)
{
    this->_rootLayout = new QVBoxLayout(this);
    this->_rootLayout->setSpacing(0);
    // HEADER
    this->_header = new EditorHeader;
    // BODY
    this->_extractEditTab = new ExtractionEditTab(api);
    this->_cleanEditTab = new CleanEditTab(api);
    this->_editEditTab = new EditorEditTab(api);
    this->_saveEditTab = new SaveEditTab(api);

    FuncNetCall func = bind(&CleanEditTab::loadAPI, _cleanEditTab);
    this->_cleanPropTab = new CleanPropertyTab(func);
    func = bind(&EditorEditTab::loadAPI, _editEditTab);
    this->_editPropTab = new EditPropertyTab(func);
    func = bind(&ExtractionEditTab::loadAPI, _extractEditTab);
    this->_extractPropTab = new ExtractionPropertyTab(func);
    func = [](){}; // Do nothing
    this->_savePropTab = new SavePropertyTab(func);

    this->_stackEdit = new QStackedWidget;
    this->_stackEdit->addWidget(_extractEditTab);
    this->_stackEdit->addWidget(_cleanEditTab);
    this->_stackEdit->addWidget(_editEditTab);
    this->_stackEdit->addWidget(_saveEditTab);
    this->_stackProp = new QStackedWidget;
    this->_stackProp->setFixedWidth(EDITOR_PROPTAB_WIDTH);
    this->_stackProp->addWidget(_extractPropTab);
    this->_stackProp->addWidget(_cleanPropTab);
    this->_stackProp->addWidget(_editPropTab);
    this->_stackProp->addWidget(_savePropTab);

    this->_sourcePages = new ImageViewer;

    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    this->_rootLayout->addWidget(_header);
    // BODY
    QHBoxLayout *bodyLayout = new QHBoxLayout;

    bodyLayout->addWidget(_sourcePages);
    bodyLayout->addWidget(_stackEdit);
    bodyLayout->addWidget(_stackProp);
    this->_rootLayout->addLayout(bodyLayout);
    // Style
    this->_rootLayout->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(EDITOR_MIN_SIZE);
    parent->resize(EDITOR_DEF_SIZE);
}
