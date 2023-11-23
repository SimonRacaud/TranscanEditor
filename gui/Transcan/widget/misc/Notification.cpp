#include "Notification.h"
#include "include/env_config.h"

#include <QVBoxLayout>
#include <QGraphicsOpacityEffect>
#include <QPropertyAnimation>

Notification::Notification(QString const &message, QWidget *parent, Type type) : QFrame(parent)
{
    this->_type = type;
    this->setObjectName("Notification");

    this->_message = new QLabel("⚠ " + message);
    _message->setAlignment(Qt::AlignCenter);
    _message->setObjectName("NotificationLabel");

    auto *lay = new QVBoxLayout;
    lay->addWidget(_message);
    this->setLayout(lay);

    // Animation : Create fade:
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    this->setGraphicsEffect(effect);
    QPropertyAnimation *a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(NOTIF_TRANSIT_TIME); // ms
    a->setStartValue(0);
    a->setEndValue(1);
    a->setEasingCurve(QEasingCurve::InBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);

    // Planify fadeOut animation:
    connect(&_timer, &QTimer::timeout, this, &Notification::fadeOut);
    _timer.start(NOTIF_TRANSIT_TIME + NOTIF_SHOW_TIME); // 1000 ms to make the notification opacity full and 1000 seconds to call the fade out so total of 2000ms.

    // Configure widget
    this->resize(_message->fontMetrics().horizontalAdvance(message) * 1.2, 50);
    this->move(NOTIF_POS);
    this->raise();
    this->show();

    // Style
    if (type == Type::ERROR) {
        this->setStyleSheet("background-color: #E63946;");
    } else {
        this->setStyleSheet("background-color: #457B9D;");
    }
}

void Notification::Build(QString const &message, QWidget *parent, Type type)
{
    new Notification(message, parent, type);
}

void Notification::fadeOut()
{
    QGraphicsOpacityEffect *effect = new QGraphicsOpacityEffect();
    this->setGraphicsEffect(effect);

    QPropertyAnimation *a = new QPropertyAnimation(effect, "opacity");
    a->setDuration(NOTIF_TRANSIT_TIME); // it will took 1000ms to face out
    a->setStartValue(1);
    a->setEndValue(0);
    a->setEasingCurve(QEasingCurve::OutBack);
    a->start(QPropertyAnimation::DeleteWhenStopped);
    connect(a, &QPropertyAnimation::finished, this, &Notification::hide);
}

void Notification::hide()
{
    QFrame::hide();
    this->deleteLater();
}
