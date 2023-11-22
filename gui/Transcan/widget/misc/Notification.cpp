#include "Notification.h"
#include "include/env_config.h"

#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

Notification::Notification(QString const &message, QWidget *parent) : QWidget(parent)
{
    auto *lay = new QVBoxLayout;
    this->_message = new QLabel(message);
    _message->setStyleSheet("border-radius:15px; background-color: #ffffff; color:#505050");
    _message->setAlignment(Qt::AlignCenter);
    _message->setObjectName("NotificationLabel");
    lay->addWidget(_message);
    this->setLayout(lay);
    // Create fade in annocation:
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    _message->setGraphicsEffect(effect);
    QPropertyAnimation *a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(NOTIF_TRANSIT_TIME); // ms
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);

    // Planify fadeOut annimation:
    connect(&_timer, &QTimer::timeout, this, &Notification::fadeOut);
    _timer.start(NOTIF_TRANSIT_TIME + NOTIF_SHOW_TIME); // 1000 ms to make the notification opacity full and 1000 seconds to call the fade out so total of 2000ms.

    // Configure widget
    this->resize(_message->fontMetrics().horizontalAdvance(message) * 1.2, 70);
    this->move(NOTIF_POS);
    this->raise();
    this->show();
}

void Notification::Build(QString const &message, QWidget *parent)
{
    new Notification(message, parent);
}

void Notification::fadeOut()
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    _message->setGraphicsEffect(effect);

    QPropertyAnimation *a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(NOTIF_TRANSIT_TIME); // it will took 1000ms to face out
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a, &QPropertyAnimation::finished, _message, &QLabel::hide);
    this->deleteLater();
}
