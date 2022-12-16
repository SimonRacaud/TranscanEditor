#include "SelectAreaRect.h"

#include <QPainter>

SelectAreaRect::SelectAreaRect(BlockCluster const &cluster)
    : _data(cluster), _selected(_data.cleanBox)
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
    return _data.polygon.boundingRect();
}

bool SelectAreaRect::isSelected() const
{
    return _selected;
}

BlockCluster const &SelectAreaRect::getData()
{
    this->_data.cleanBox = _selected; // Update state
    return _data;
}

bool SelectAreaRect::isOnArea(QRectF area) const
{
    return area.contains(this->boundingRect());
}

/** Protected **/

void SelectAreaRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    _selected = !_selected;
    update();
}
