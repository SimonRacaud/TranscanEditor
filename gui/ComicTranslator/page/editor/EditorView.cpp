#include "EditorView.h"

#include <QHBoxLayout>

EditorView::EditorView(QWidget *parent)
    : QWidget{parent}, _rootLayout(nullptr), _showSourceButton(new QPushButton), _extractButton(new QPushButton),
      _cleanButton(new QPushButton), _editButton(new QPushButton), _saveButton(new QPushButton),
      _exitButton(new QPushButton)
{
    this->_rootLayout = new QVBoxLayout(this);
    this->setupHeader();
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
    middleHeaderLay->setAlignment(Qt::AlignmentFlag::AlignCenter);
    _extractButton->setText("EXTRACT");
    middleHeaderLay->addWidget(_extractButton);
    _cleanButton->setText("CLEAN");
    middleHeaderLay->addWidget(_cleanButton);
    _editButton->setText("EDIT");
    middleHeaderLay->addWidget(_editButton);
    _saveButton->setText("SAVE");
    middleHeaderLay->addWidget(_saveButton);
    // Right layout
    rightHeaderLay->setAlignment(Qt::AlignmentFlag::AlignRight);
    _exitButton->setText("Exit");
    rightHeaderLay->addWidget(_exitButton);
    //
    rootHeaderLay->addLayout(leftHeaderLay);
    rootHeaderLay->addLayout(middleHeaderLay);
    rootHeaderLay->addLayout(rightHeaderLay);
    this->_rootLayout->addLayout(rootHeaderLay);
}

