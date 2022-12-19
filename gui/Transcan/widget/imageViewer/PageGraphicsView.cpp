#include "PageGraphicsView.h"
#include <QScrollBar>
#include <QMouseEvent>

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
    QGraphicsView::resizeEvent(event);
    emit this->resizedSignal();
}

void PageGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QGraphicsView::mouseDoubleClickEvent(event);
    emit this->onDoubleClick(event);
}
