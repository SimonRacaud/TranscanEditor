#ifndef QPAGEVIEWER_H
#define QPAGEVIEWER_H

#include <QGraphicsView>

class PageGraphicsView : public QGraphicsView
{
    Q_OBJECT
public:
    PageGraphicsView();

signals:
    void resizedSignal();

    void onDoubleClick(QMouseEvent *event);

private:
    virtual void resizeEvent(QResizeEvent *event) override;

    virtual void mouseDoubleClickEvent(QMouseEvent *event) override;
};

#endif // QPAGEVIEWER_H
