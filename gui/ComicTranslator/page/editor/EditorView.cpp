#include "EditorView.h"
#include "widget/editor/property/ExtractionPropertyTab.h"
#include "widget/editor/property/CleanPropertyTab.h"
#include "widget/editor/property/EditPropertyTab.h"
#include "widget/editor/property/SavePropertyTab.h"

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
    this->_editArea = new AEditArea; // DEBUG temp
    this->_propertyTab = new SavePropertyTab; // DEBUG temp
    QHBoxLayout *bodyLayout = new QHBoxLayout;

    bodyLayout->addWidget(_editArea);
    bodyLayout->addWidget(_propertyTab);
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

