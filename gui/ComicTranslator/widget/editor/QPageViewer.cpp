#include "QPageViewer.h"

QPageViewer::QPageViewer()
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setStyleSheet("background: transparent");
    this->show();
}

/** Internal methods **/

void QPageViewer::resizeEvent(QResizeEvent *event)
{
    emit this->resizedSignal();
}
