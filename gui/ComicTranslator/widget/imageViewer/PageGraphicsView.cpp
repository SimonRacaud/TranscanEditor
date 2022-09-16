#include "PageGraphicsView.h"
#include <QScrollBar>

PageGraphicsView::PageGraphicsView()
{
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAsNeeded);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->verticalScrollBar()->setFixedWidth(15);
    this->setStyleSheet("background: transparent");
    this->show();
}

/** Internal methods **/

void PageGraphicsView::resizeEvent(QResizeEvent *event)
{
    emit this->resizedSignal();
}
