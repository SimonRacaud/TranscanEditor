#include "APropertyTab.h"

APropertyTab::APropertyTab(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *rootLayout = new QVBoxLayout;
    QVBoxLayout *upperLayout = new QVBoxLayout;
    QVBoxLayout *lowerLayout = new QVBoxLayout;

    // Upper section (properties)
    this->_title = new QLabel();
    this->_title->setText("Properties:");
    this->_title->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    upperLayout->setSpacing(EDITOR_PROP_SPACING);
    upperLayout->addWidget(_title);
    upperLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    QWidget *propertiesContainer = new QWidget;
    this->_propertiesLayout = new QVBoxLayout(propertiesContainer);
    this->_propertiesLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_propScollArea = new QScrollArea;
    this->_propScollArea->setStyleSheet("QScrollArea { border: none; }");
    this->_propScollArea->setWidgetResizable(true);
    this->_propScollArea->setWidget(propertiesContainer);
    this->_propScollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->_propScollArea->setBackgroundRole(QPalette::Base);
    upperLayout->addWidget(_propScollArea);
    // Lower section (help)
    this->_helpTitle = new QLabel();
    this->_helpTitle->setText("Help:");
    this->_helpTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    lowerLayout->setSpacing(EDITOR_PROP_SPACING);
    lowerLayout->addWidget(_helpTitle);
    lowerLayout->setAlignment(Qt::AlignmentFlag::AlignBottom);
    this->_help = new QLabel();
    this->_help->setMinimumHeight(EDITOR_PROP_HELP_MIN_HEIGHT);
    this->_help->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_help->setWordWrap(true);
    lowerLayout->addWidget(_help);
    this->_nextButton = new QPushButton("Next step");
    lowerLayout->addWidget(_nextButton);
    //
    rootLayout->addLayout(upperLayout);
    rootLayout->addLayout(lowerLayout);
    this->setLayout(rootLayout);
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_PROPTAB_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setFixedWidth(EDITOR_PROPTAB_WIDTH);
}
