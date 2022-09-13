#ifndef QPAGEVIEWER_H
#define QPAGEVIEWER_H

#include <QGraphicsView>

class QPageViewer : public QGraphicsView
{
    Q_OBJECT
public:
    QPageViewer();

signals:
    void resizedSignal();

private:
    virtual void resizeEvent(QResizeEvent *event) override;
};

#endif // QPAGEVIEWER_H
