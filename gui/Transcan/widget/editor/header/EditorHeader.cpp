#include "EditorHeader.h"

#include "window/MainWindow.h"
#include "include/env_style.h"

#include <QHBoxLayout>

extern MainWindow *mainWindow;

EditorHeader::EditorHeader(QWidget *)
    : QWidget{}, _showSourceButton(new QPushButton), _extractButton(new QPushButton),
      _cleanButton(new QPushButton), _editButton(new QPushButton), _saveButton(new QPushButton),
      _exitButton(new QPushButton)
{
    this->setObjectName("EditorHeader");
    QHBoxLayout *rootHeaderLay = new QHBoxLayout;
    QHBoxLayout *leftHeaderLay = new QHBoxLayout;
    QHBoxLayout *middleHeaderLay = new QHBoxLayout;
    QHBoxLayout *rightHeaderLay = new QHBoxLayout;

    // Left layout
    leftHeaderLay->setAlignment(Qt::AlignmentFlag::AlignLeft);
    _showSourceButton->setObjectName("EditorHeadShowSourceButton");
    _showSourceButton->setCheckable(true);
    this->setShowSourceButtonState(false);
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
    middleHeaderLay->setContentsMargins(0, 0, 0, 0);
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
    _exitButton->setText("Quit");
    rightHeaderLay->addWidget(_exitButton);
    //
    rootHeaderLay->addLayout(leftHeaderLay);
    rootHeaderLay->addWidget(middleHeaderWidget);
    rootHeaderLay->addLayout(rightHeaderLay);
    rootHeaderLay->setContentsMargins(EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN, EDITOR_HEAD_MARGIN);
    this->setLayout(rootHeaderLay);

    // Set background color
    QPalette pal = QPalette(QColor(0x1D3557));
    this->setAutoFillBackground(true);
    this->setPalette(pal);

    // EVENTS
    QObject::connect(_showSourceButton, &QPushButton::toggled, this, &EditorHeader::showSourceButtonClickedSlot);
    QObject::connect(_extractButton, &QPushButton::clicked, this, &EditorHeader::extractButtonClickedSlot);
    QObject::connect(_cleanButton, &QPushButton::clicked, this, &EditorHeader::cleanButtonClickedSlot);
    QObject::connect(_editButton, &QPushButton::clicked, this, &EditorHeader::editButtonClickedSlot);
    QObject::connect(_saveButton, &QPushButton::clicked, this, &EditorHeader::saveButtonClickedSlot);
    QObject::connect(_exitButton, &QPushButton::clicked, this, &EditorHeader::exitButtonClickedSlot);
}

void EditorHeader::setSelectionTabHeader(EditorTab currentTab)
{
    _extractButton->setEnabled(!(currentTab == EditorTab::EXTRACT));
    _cleanButton->setEnabled(!(currentTab == EditorTab::CLEAN));
    _editButton->setEnabled(!(currentTab == EditorTab::EDIT));
    _saveButton->setEnabled(!(currentTab == EditorTab::SAVE));
}

void EditorHeader::setShowSourceButtonState(bool enable)
{
    if (!enable) {
        this->_showSourceButton->setText("Show source ▶");
    } else {
        this->_showSourceButton->setText("Hide source ◀");
    }
    this->_showSourceButton->setChecked(enable);
}

/** SLOTS **/

void EditorHeader::showSourceButtonClickedSlot(bool checked)
{
    // show/hide source document tab
    emit this->sigShowPageSourceTab(checked);
}
void EditorHeader::extractButtonClickedSlot(bool)
{
    emit this->sigChangeTab(EditorTab::EXTRACT);
}
void EditorHeader::cleanButtonClickedSlot(bool)
{
    emit this->sigChangeTab(EditorTab::CLEAN);
}
void EditorHeader::editButtonClickedSlot(bool)
{
    emit this->sigChangeTab(EditorTab::EDIT);
}
void EditorHeader::saveButtonClickedSlot(bool)
{
    emit this->sigChangeTab(EditorTab::SAVE);
}
void EditorHeader::exitButtonClickedSlot(bool)
{
    // Go back to home page
    mainWindow->setPage(Page::HOME);
}

