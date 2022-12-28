#include "EditorView.h"
#include "widget/editor/propertyTab/ExtractionPropertyTab.h"
#include "widget/editor/propertyTab/CleanPropertyTab.h"
#include "widget/editor/propertyTab/EditPropertyTab.h"
#include "widget/editor/propertyTab/SavePropertyTab.h"
#include "widget/editor/editTab/ExtractionEditArea.h"

#include <QHBoxLayout>
#include <QtSvgWidgets/QSvgWidget>

using std::bind;

EditorView::EditorView(APIClient &api, QWidget *parent)
    : QWidget{parent}, _rootLayout(nullptr), _showSourceButton(new QPushButton), _extractButton(new QPushButton),
      _cleanButton(new QPushButton), _editButton(new QPushButton), _saveButton(new QPushButton),
      _exitButton(new QPushButton)
{
    this->_rootLayout = new QVBoxLayout(this);
    this->_rootLayout->setSpacing(0);
    this->setupHeader();
    // BODY
    this->_extractEditTab = new ExtractionEditArea(api);
    this->_cleanEditTab = new CleanEditArea(api);
    this->_editEditTab = new EditorEditArea(api);
    this->_saveEditTab = new SaveEditArea(api);

    FuncNetCall func = bind(&CleanEditArea::loadAPI, _cleanEditTab);
    this->_cleanPropTab = new CleanPropertyTab(func);
    func = bind(&EditorEditArea::loadAPI, _editEditTab);
    this->_editPropTab = new EditPropertyTab(func);
    func = bind(&ExtractionEditArea::loadAPI, _extractEditTab);
    this->_extractPropTab = new ExtractionPropertyTab(func);
    func = bind(&SaveEditArea::loadAPI, _saveEditTab);
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
    //
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

void EditorView::setupHeader()
{
    QWidget *headerContainer = new QWidget(this);
    headerContainer->setObjectName("EditorHeaderContainer");
    QHBoxLayout *rootHeaderLay = new QHBoxLayout;
    QHBoxLayout *leftHeaderLay = new QHBoxLayout;
    QHBoxLayout *middleHeaderLay = new QHBoxLayout;
    QHBoxLayout *rightHeaderLay = new QHBoxLayout;

    // Left layout
    leftHeaderLay->setAlignment(Qt::AlignmentFlag::AlignLeft);
    _showSourceButton->setText("Show source");
    _showSourceButton->setObjectName("EditorHeadShowSourceButton");
    _showSourceButton->setCheckable(true);
    leftHeaderLay->addWidget(_showSourceButton);
    // Middle layout
    this->_separator1 = new QSvgWidget(EDITOR_HEAD_SEPARATOR);
    this->_separator2 = new QSvgWidget(EDITOR_HEAD_SEPARATOR);
    this->_separator3 = new QSvgWidget(EDITOR_HEAD_SEPARATOR);
    _separator1->setFixedHeight(EDITOR_HEAD_SEPARATOR_HEIGHT);
    _separator2->setFixedHeight(EDITOR_HEAD_SEPARATOR_HEIGHT);
    _separator3->setFixedHeight(EDITOR_HEAD_SEPARATOR_HEIGHT);
    QWidget *middleHeaderWidget = new QWidget();
    middleHeaderWidget->setObjectName("MiddleHeaderWidget");
    middleHeaderWidget->setLayout(middleHeaderLay);
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
    rootHeaderLay->addWidget(middleHeaderWidget);
    rootHeaderLay->addLayout(rightHeaderLay);
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    rootHeaderLay->setContentsMargins(EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN);
    headerContainer->setLayout(rootHeaderLay);
    this->_rootLayout->addWidget(headerContainer);
}

void EditorView::setSelectionTabHeader()
{
    EditorTab currentTab = (EditorTab)this->_stackEdit->currentIndex();

    _extractButton->setEnabled(!(currentTab == EditorTab::EXTRACT));
    _cleanButton->setEnabled(!(currentTab == EditorTab::CLEAN));
    _editButton->setEnabled(!(currentTab == EditorTab::EDIT));
    _saveButton->setEnabled(!(currentTab == EditorTab::SAVE));
}
