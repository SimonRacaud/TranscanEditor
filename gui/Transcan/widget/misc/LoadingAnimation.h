#ifndef LOADINGANIMATION_H
#define LOADINGANIMATION_H

#include <QLabel>
#include <QMovie>

class LoadingAnimation : public QLabel
{
public:
    LoadingAnimation(QWidget *parent = nullptr);

    void start(QSize const &windowSize);
    void stop();

    void centerPosition(QSize const &windowSize);

private:
    QMovie *_animation;
};

#endif // LOADINGANIMATION_H
