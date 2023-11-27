#include "LoadingAnimation.h"
#include <QMovie>
#include "include/env_config.h"
#include "include/env_style.h"

LoadingAnimation::LoadingAnimation(QWidget *parent) : QLabel(parent)
{
    this->_animation = new QMovie();
    this->_animation->setFileName(IMG_LOADING);
    this->setMovie(_animation);
    if (parent != nullptr) { // Floating animation
        this->resize(LOADING_ICON_SIZE_L, LOADING_ICON_SIZE_L);
        _animation->setScaledSize(QSize(LOADING_ICON_SIZE_L, LOADING_ICON_SIZE_L));
    } else { // Embedded animation
        _animation->setScaledSize(QSize(LOADING_ICON_SIZE_S, LOADING_ICON_SIZE_S));
    }
}

void LoadingAnimation::start(QSize const &windowSize)
{
    if (this->parent() != nullptr) {
        this->centerPosition(windowSize);
        this->show();
    }
    this->_animation->start();
}

void LoadingAnimation::stop()
{    
    this->_animation->stop();
    if (this->parent() != nullptr) {
        this->hide();
    }
}

void LoadingAnimation::centerPosition(QSize const &windowSize)
{
    this->move(windowSize.width() / 2 - this->width() / 2,
                windowSize.height() / 2 - this->height() / 2);
}
