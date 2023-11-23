#include "ZoomWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QGraphicsDropShadowEffect>
#include "include/env_config.h"
#include "include/env_style.h"

ZoomWidget::ZoomWidget(float zoom, QWidget *parent)
    : QFrame(parent), _zoom(zoom)
{
    this->setObjectName("ZoomWidget");

    QLabel *zoomLabel = new QLabel(tr("zoom"));
    this->_level = new QLabel();
    _level->setFixedWidth(40);
    this->_slider = new QSlider(Qt::Orientation::Horizontal);
    _slider->setMinimum(ZOOM_MIN_INT);
    _slider->setMaximum(ZOOM_MAX_INT);
    //
    auto rootVLay = new QHBoxLayout;
    rootVLay->setContentsMargins(0, 0, 0, 0);
    rootVLay->addWidget(zoomLabel);
    rootVLay->addWidget(_slider);
    rootVLay->addWidget(_level);
    this->setLayout(rootVLay);
    //
    this->setZoom(zoom);
    // Event
    connect(_slider, &QSlider::valueChanged, this, &ZoomWidget::sliderMoved);

    // STYLE
    QGraphicsDropShadowEffect* shadowEffect = new QGraphicsDropShadowEffect();
    shadowEffect->setBlurRadius(STYLE_SHADOW_RADIUS_S);
    shadowEffect->setXOffset(STYLE_SHADOW_OFFSET);
    shadowEffect->setYOffset(STYLE_SHADOW_OFFSET);
    shadowEffect->setColor(STYLE_SHADOW_COLOR);
    this->setGraphicsEffect(shadowEffect);
}

/** PUBLIC SLOT **/

void ZoomWidget::setZoom(float value)
{
    if (value < 0 || value > 5) {
        throw std::invalid_argument("ZoomWidget::setZoom expected interval 0. to 4.");
    }
    this->_zoom = value;
    this->_slider->setSliderPosition((int)(_zoom * 100.f));
    this->_level->setText(QString::number((int)(value * 100.f)) + "%");
}

/** PRIVATE SLOT **/

void ZoomWidget::sliderMoved(int value)
{
    this->setZoom((float)value / 100.f);
    emit this->sigZoomChanged(_zoom);
}
