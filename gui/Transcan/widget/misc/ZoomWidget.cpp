#include "ZoomWidget.h"

#include <QVBoxLayout>
#include <QHBoxLayout>
#include "include/environment.h"

ZoomWidget::ZoomWidget(float zoom, QWidget *parent)
    : QWidget{parent}, _zoom(zoom)
{
    this->_level = new QLabel();
    _level->setFixedWidth(40);
    this->_slider = new QSlider(Qt::Orientation::Horizontal);
    _slider->setMinimum(ZOOM_MIN_INT);
    _slider->setMaximum(ZOOM_MAX_INT);
    //
    auto rootVLay = new QHBoxLayout;
    rootVLay->addWidget(_level);
    rootVLay->addWidget(_slider);
    this->setLayout(rootVLay);
    //
    this->setZoom(zoom);
    // Event
    connect(_slider, &QSlider::valueChanged, this, &ZoomWidget::sliderMoved);
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