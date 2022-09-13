#include "SelectAreaRect.h"

#include <QPainter>

SelectAreaRect::SelectAreaRect(BlockCluster const &cluster)
    : _data(cluster)
{
    this->setZValue(8);
}

/** Public **/

void SelectAreaRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = this->boundingRect();
    QColor borderColor = (_selected) ? Qt::black : Qt::red;
    QPen pen(borderColor, 2 /* Line width */, Qt::DashDotLine);

    painter->setPen(pen);
    if (_selected) {
        painter->setBrush(QColor(255, 255, 255, 125)); // background color
    } else {
        painter->setBrush(Qt::transparent); // background color
    }
    painter->drawRect(rect);
}

QRectF SelectAreaRect::boundingRect() const
{
    return QRectF(_data.box.x(), _data.box.y(),
                  _data.box.width(), _data.box.height());
}

bool SelectAreaRect::isSelected() const
{
    return _selected;
}

BlockCluster const &SelectAreaRect::getData() const
{
    return _data;
}

/** Protected **/

void SelectAreaRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _selected = !_selected;
    update();
}
