#include "APropertyTab.h"
#include <QVBoxLayout>

APropertyTab::APropertyTab(QWidget *parent)
    : QWidget{parent}
{
    QVBoxLayout *rootLayout = new QVBoxLayout;
    QVBoxLayout *upperLayout = new QVBoxLayout;
    QVBoxLayout *lowerLayout = new QVBoxLayout;

    this->_title = new QLabel();
    this->_title->setText("Properties:");
    this->_title->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    upperLayout->addWidget(_title);
    upperLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    //
    this->_helpTitle = new QLabel();
    this->_helpTitle->setText("Help:");
    this->_helpTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    lowerLayout->addWidget(_helpTitle);
    lowerLayout->setAlignment(Qt::AlignmentFlag::AlignBottom);
    this->_help = new QLabel();
    this->_help->setMinimumHeight(EDITOR_PROP_MIN_HEIGHT);
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
    this->setMinimumWidth(EDITOR_PROPTAB_MIN_WIDTH);
}
