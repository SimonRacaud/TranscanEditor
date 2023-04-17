#include "APropertyTab.h"
#include <QMessageBox>
#include <iostream>
#include <QScrollBar>

APropertyTab::APropertyTab(FuncNetCall &reloadFunc, QWidget *parent)
    : QWidget{parent}, _netCallback(reloadFunc)
{
    QVBoxLayout *rootLayout = new QVBoxLayout;
    QVBoxLayout *upperLayout = new QVBoxLayout;
    QVBoxLayout *lowerLayout = new QVBoxLayout;

    // Zoom
    _zoomWidget = new ZoomWidget(0);
    rootLayout->addWidget(_zoomWidget);
    connect(_zoomWidget, &ZoomWidget::sigZoomChanged, [this](float value){
        emit this->sigChangeZoom(value);
    });
    // Upper section (properties)
    this->_title = new QLabel();
    this->_title->setObjectName("EditorPropTitle");
    this->_title->setText("Properties:");
    this->_title->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    this->_reloadButton = new QPushButton("Reload");
    upperLayout->setSpacing(EDITOR_PROP_SPACING);
    upperLayout->addWidget(_title);
    upperLayout->addWidget(_reloadButton);
    upperLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    QWidget *propertiesContainer = new QWidget;
    this->_propertiesLayout = new QVBoxLayout(propertiesContainer);
    this->_propertiesLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_propScollArea = new QScrollArea;
    this->_propScollArea->horizontalScrollBar()->setEnabled(false);
    this->_propScollArea->setStyleSheet("QScrollArea { border: none; }");
    this->_propScollArea->setWidgetResizable(true);
    this->_propScollArea->setWidget(propertiesContainer);
    this->_propScollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->_propScollArea->setBackgroundRole(QPalette::Base);
    upperLayout->addWidget(_propScollArea);

    // Lower section (help)
    this->_helpTitle = new QLabel();
    this->_helpTitle->setText("Help:");
    this->_helpTitle->setObjectName("EditorHelpTitle");
    this->_helpTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    lowerLayout->setSpacing(EDITOR_PROP_SPACING);
    lowerLayout->addWidget(_helpTitle);
    lowerLayout->setAlignment(Qt::AlignmentFlag::AlignBottom);
    this->_help = new QLabel();
    this->_help->setObjectName("EditorHelpText");
    this->_help->setMinimumHeight(EDITOR_PROP_HELP_MIN_HEIGHT);
    this->_help->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_help->setWordWrap(true);
    lowerLayout->addWidget(_help);
    QHBoxLayout *nextButtonLay = new QHBoxLayout();
    this->_nextButton = new QPushButton("Next step");
    this->_nextButton->setObjectName("EditorNextStepButton");
    nextButtonLay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    nextButtonLay->addWidget(_nextButton);
    nextButtonLay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    lowerLayout->addLayout(nextButtonLay);
    //
    rootLayout->addLayout(upperLayout);
    rootLayout->addLayout(lowerLayout);
    this->setLayout(rootLayout);
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_PROPTAB_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setFixedWidth(EDITOR_PROPTAB_WIDTH);
    // EVENT
    connect(_nextButton, &QPushButton::clicked, [this]() { emit this->nextStep(); });
    FuncNetCall &reloadCallback = _netCallback;
    connect(_reloadButton, &QPushButton::clicked, [reloadCallback, this]() {
        auto box = QMessageBox::question(this, "Action confirmation", "This action will overwrite the current modifications. Continue?");
        if (box == QMessageBox::Yes) {
            try {
                this->_reloadButton->setEnabled(false);
                reloadCallback(); // Idea: Signal when all requests are fulfilled
            } catch (std::runtime_error const &err) {
                std::cerr << "Fail to reload. " << err.what();
            }
        }
    });    
}

void APropertyTab::unlockReloadButton()
{
    this->_reloadButton->setEnabled(true);
}

 void APropertyTab::setZoom(float value)
 {
     this->_zoomWidget->setZoom(value);
 }
