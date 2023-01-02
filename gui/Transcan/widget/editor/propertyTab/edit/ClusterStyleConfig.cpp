#include "ClusterStyleConfig.h"

#include <QFormLayout>
#include <QLabel>
#include "include/environment.h"

ClusterStyleConfig::ClusterStyleConfig(QString const &title) : QFrame()
{
    QLabel *titleWidget = new QLabel(title);
    titleWidget->setObjectName("EditorEditPropClusterStyleConfigTitle");
    _inLineHeight = new QSpinBox();
    _inLineHeight->setMaximum(100);
    _inLineHeight->setMinimum(1);
    _inLineHeight->setSingleStep(5);

    _inStrokeWidth = new QSpinBox();
    _inStrokeWidth->setMaximum(9);
    _inStrokeWidth->setMinimum(1);
    _inStrokeWidth->setSingleStep(1);

    _inFont = new QFontComboBox();

    _selectColorButton = new QPushButton(tr("Select"));
    _colorSelect = new QColorDialog();

    QFormLayout *layout = new QFormLayout();
    layout->addRow(tr("Line height:"), _inLineHeight);
    layout->addRow(tr("Stroke width:"), _inStrokeWidth);
    layout->addRow(tr("Font family:"), _inFont);
    layout->addRow(tr("Text color:"), _selectColorButton);

    QVBoxLayout *rootLay = new QVBoxLayout(this);
    rootLay->addWidget(titleWidget);
    rootLay->addLayout(layout);

    connect(_selectColorButton, &QPushButton::clicked, this, &ClusterStyleConfig::onSelectColorButtonClicked);

    this->setFixedWidth(EDITOR_PROPTAB_WIDTH * 0.85f);

    connect(_inStrokeWidth, &QSpinBox::valueChanged, [this](){ emit this->onUpdate(); });
    connect(_colorSelect, &QDialog::finished, [this](){ emit this->onUpdate(); });
    connect(_inFont, &QFontComboBox::currentIndexChanged, [this](){ emit this->onUpdate(); });
    connect(_inLineHeight, &QSpinBox::valueChanged, [this](){ emit this->onUpdate(); });
}

void ClusterStyleConfig::setCluster(BlockCluster const &cluster)
{
    this->_data.color = cluster.style.color;
    this->_data.font = cluster.style.font;
    this->_data.lineHeight = cluster.style.lineHeight;
    this->_data.strokeWidth = cluster.style.strokeWidth;
    this->updateView();
}

void ClusterStyleConfig::setConfig(RenderConfig const &config)
{
    this->_data = config;
    this->updateView();
}

RenderConfig ClusterStyleConfig::getConfig()
{
    // update _data
    _data.color = _colorSelect->currentColor();
    _data.font = _inFont->currentFont();
    _data.lineHeight = _inLineHeight->value();
    _data.strokeWidth = _inStrokeWidth->value();
    return _data;
}

/* PRIVATE */

void ClusterStyleConfig::connectEvents()
{
    connect(_inStrokeWidth, &QSpinBox::valueChanged, [this](){ emit this->onUpdate(); });
    connect(_colorSelect, &QDialog::finished, [this](){ emit this->onUpdate(); });
    connect(_inFont, &QFontComboBox::currentIndexChanged, [this](){ emit this->onUpdate(); });
    connect(_inLineHeight, &QSpinBox::valueChanged, [this](){ emit this->onUpdate(); });
}
void ClusterStyleConfig::disconnectEvents()
{
    disconnect(_inStrokeWidth, &QSpinBox::valueChanged, nullptr, nullptr);
    disconnect(_colorSelect, &QDialog::finished, nullptr, nullptr);
    disconnect(_inFont, &QFontComboBox::currentIndexChanged, nullptr, nullptr);
    disconnect(_inLineHeight, &QSpinBox::valueChanged, nullptr, nullptr);
}

void ClusterStyleConfig::onSelectColorButtonClicked(bool)
{
    this->_colorSelect->open();
}

void ClusterStyleConfig::updateView()
{
    this->disconnectEvents(); // Avoid emitting the update signal
    this->_colorSelect->setCurrentColor(_data.color);
    this->_inFont->setCurrentFont(_data.font);
    this->_inLineHeight->setValue(_data.lineHeight);
    this->_inStrokeWidth->setValue(_data.strokeWidth);
    this->connectEvents();
}
