#include "EditorView.h"
#include "widget/editor/propertyTab/ExtractionPropertyTab.h"
#include "widget/editor/propertyTab/CleanPropertyTab.h"
#include "widget/editor/propertyTab/EditPropertyTab.h"
#include "widget/editor/propertyTab/SavePropertyTab.h"
#include "widget/editor/editTab/ExtractionEditArea.h"

#include <QHBoxLayout>

EditorView::EditorView(QWidget *parent)
    : QWidget{parent}, _rootLayout(nullptr), _showSourceButton(new QPushButton), _extractButton(new QPushButton),
      _cleanButton(new QPushButton), _editButton(new QPushButton), _saveButton(new QPushButton),
      _exitButton(new QPushButton)
{
    this->_rootLayout = new QVBoxLayout(this);
    this->_rootLayout->setSpacing(0);
    this->setupHeader();
    //
    this->_cleanPropTab = new CleanPropertyTab;
    this->_editPropTab = new EditPropertyTab;
    this->_extractPropTab = new ExtractionPropertyTab;
    this->_savePropTab = new SavePropertyTab;

    this->_extractEditTab = new ExtractionEditArea;
    this->_cleanEditTab = new CleanEditArea;
    this->_editEditTab = new EditorEditArea;
    this->_saveEditTab = new SaveEditArea;

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
    //
    QHBoxLayout *bodyLayout = new QHBoxLayout;

    bodyLayout->addWidget(_stackEdit);
    bodyLayout->addWidget(_stackProp);
    this->_rootLayout->addLayout(bodyLayout);
    // Style
    this->_rootLayout->setContentsMargins(0, 0, 0, 0);
    this->setMinimumSize(EDITOR_MIN_SIZE);
    this->resize(EDITOR_DEF_SIZE);
}

void EditorView::setupHeader()
{
    QHBoxLayout *rootHeaderLay = new QHBoxLayout;
    QHBoxLayout *leftHeaderLay = new QHBoxLayout;
    QHBoxLayout *middleHeaderLay = new QHBoxLayout;
    QHBoxLayout *rightHeaderLay = new QHBoxLayout;

    // Left layout
    leftHeaderLay->setAlignment(Qt::AlignmentFlag::AlignLeft);
    _showSourceButton->setText("Show source");
    leftHeaderLay->addWidget(_showSourceButton);
    // Middle layout
    this->_separator1 = new QLabel(">");
    this->_separator2 = new QLabel(">");
    this->_separator3 = new QLabel(">");
    middleHeaderLay->setAlignment(Qt::AlignmentFlag::AlignCenter);
    _extractButton->setText("EXTRACT");
    middleHeaderLay->addWidget(_extractButton);
    middleHeaderLay->addWidget(_separator1);
    _cleanButton->setText("CLEAN");
    middleHeaderLay->addWidget(_cleanButton);
    middleHeaderLay->addWidget(_separator2);
    _editButton->setText("EDIT");
    middleHeaderLay->addWidget(_editButton);
    middleHeaderLay->addWidget(_separator3);
    _saveButton->setText("SAVE");
    middleHeaderLay->addWidget(_saveButton);
    middleHeaderLay->setSpacing(EDITOR_HEAD_MIDDLE_SPACING);
    // Right layout
    rightHeaderLay->setAlignment(Qt::AlignmentFlag::AlignRight);
    _exitButton->setText("Exit");
    rightHeaderLay->addWidget(_exitButton);
    //
    rootHeaderLay->addLayout(leftHeaderLay);
    rootHeaderLay->addLayout(middleHeaderLay);
    rootHeaderLay->addLayout(rightHeaderLay);
    this->_rootLayout->addLayout(rootHeaderLay);
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    rootHeaderLay->setContentsMargins(EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN);
}

