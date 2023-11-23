#ifndef NOTIFICATION_H
#define NOTIFICATION_H

#include <QFrame>
#include <QLabel>
#include <QTimer>

/**
 * @brief The Notification class : Briefly show a message on the user's screen
 * Useful to show error messages to the user
 */
class Notification : public QFrame
{
    Q_OBJECT
public:
    enum Type {
        ERROR,
        INFO
    };

    Notification(QString const &message, QWidget *parent, Type type);

public:
    static void Build(QString const &message, QWidget *parent, Type type = Type::ERROR);

private slots:
    void fadeOut();

    virtual void hide();
private:
    QLabel *_message;
    QTimer _timer;
    Type _type;
};

#endif // NOTIFICATION_H
