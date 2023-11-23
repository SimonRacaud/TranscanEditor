#include "APropertyTab.h"

#include "include/env_style.h"
#include <QMessageBox>
#include <iostream>
#include <QScrollBar>
#include <QGraphicsDropShadowEffect>

APropertyTab::APropertyTab(FuncNetCall &reloadFunc, QWidget *parent)
    : QWidget{parent}, _netCallback(reloadFunc)
{
    QVBoxLayout *rootLayout = new QVBoxLayout;

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

    QWidget *propertiesContainer = new QWidget();
    propertiesContainer->setContentsMargins(0, 0, 0, 0);
    propertiesContainer->setObjectName("propertiesContainer");
    this->_propertiesLayout = new QVBoxLayout(propertiesContainer);
    _propertiesLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    _propertiesLayout->setSpacing(EDITOR_PROP_SPACING);
    this->_propertiesLayout->setAlignment(Qt::AlignmentFlag::AlignTop);
    _propertiesLayout->addWidget(_title);
    _propertiesLayout->addWidget(_reloadButton);

    this->_propScollArea = new QScrollArea;
    this->_propScollArea->setObjectName("PropertiesArea");
    this->_propScollArea->horizontalScrollBar()->setEnabled(false);
    this->_propScollArea->setStyleSheet("QScrollArea { border: none; }");
    this->_propScollArea->setWidgetResizable(true);
    this->_propScollArea->setWidget(propertiesContainer);
    this->_propScollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    // Lower section (help)
    QFrame *helpContainer = new QFrame();
    helpContainer->setContentsMargins(0, 0, 0, 0);
    QVBoxLayout *helpLayout = new QVBoxLayout;
    helpContainer->setObjectName("helpContainer");
    helpContainer->setLayout(helpLayout);
    this->_helpTitle = new QLabel();
    this->_helpTitle->setText("Help:");
    this->_helpTitle->setObjectName("EditorHelpTitle");
    this->_helpTitle->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    helpLayout->setSpacing(EDITOR_PROP_SPACING);
    helpLayout->addWidget(_helpTitle);
    helpLayout->setAlignment(Qt::AlignmentFlag::AlignBottom);
    this->_help = new QLabel();
    this->_help->setObjectName("EditorHelpText");
    //this->_help->setTextFormat(Qt::TextFormat::MarkdownText);
    this->_help->setMinimumHeight(EDITOR_PROP_HELP_MIN_HEIGHT);
    this->_help->setAlignment(Qt::AlignmentFlag::AlignTop);
    this->_help->setWordWrap(true);
    helpLayout->addWidget(_help);

    QHBoxLayout *nextButtonLay = new QHBoxLayout();
    this->_nextButton = new QPushButton("Next step");
    this->_nextButton->setObjectName("EditorNextStepButton");
    nextButtonLay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    nextButtonLay->addWidget(_nextButton);
    nextButtonLay->addSpacerItem(new QSpacerItem(1, 1, QSizePolicy::Expanding));
    //
    rootLayout->addWidget(_propScollArea);
    rootLayout->addWidget(helpContainer);
    rootLayout->addSpacing(8);
    rootLayout->addLayout(nextButtonLay);
    rootLayout->addSpacing(2);
    this->setLayout(rootLayout);

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

    /// STYLE
    QGraphicsDropShadowEffect* shadowEffectProp = new QGraphicsDropShadowEffect();
    shadowEffectProp->setBlurRadius(STYLE_SHADOW_RADIUS_S);
    shadowEffectProp->setXOffset(STYLE_SHADOW_OFFSET);
    shadowEffectProp->setYOffset(STYLE_SHADOW_OFFSET);
    shadowEffectProp->setColor(STYLE_SHADOW_COLOR);
    this->_propScollArea->setGraphicsEffect(shadowEffectProp);

    QGraphicsDropShadowEffect* shadowEffectHelp = new QGraphicsDropShadowEffect();
    shadowEffectHelp->setBlurRadius(STYLE_SHADOW_RADIUS_S);
    shadowEffectHelp->setXOffset(STYLE_SHADOW_OFFSET);
    shadowEffectHelp->setYOffset(STYLE_SHADOW_OFFSET);
    shadowEffectHelp->setColor(STYLE_SHADOW_COLOR);
    helpContainer->setGraphicsEffect(shadowEffectHelp);
    // Set background color
    QPalette pal = QPalette(QColor(COLOR_LGREEN));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    this->setFixedWidth(EDITOR_PROPTAB_WIDTH);
}

void APropertyTab::unlockReloadButton()
{
    this->_reloadButton->setEnabled(true);
}

 void APropertyTab::setZoom(float value)
 {
     this->_zoomWidget->setZoom(value);
 }
