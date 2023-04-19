#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QWidget>
#include <QLabel>
#include <QTimer>

class Notification : public QWidget
{
    Q_OBJECT
public:
    Notification(QString const &message, QWidget *parent);

public:
    static void Build(QString const &message, QWidget *parent);

private slots:
    void fadeOut();

private:
    QLabel *_message;
    QTimer _timer;
};

#endif // NOTIFICATION_H
