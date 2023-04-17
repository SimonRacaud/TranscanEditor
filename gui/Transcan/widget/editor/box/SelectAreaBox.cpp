#include "SelectAreaBox.h"

#include <QPainter>

SelectAreaBox::SelectAreaBox(BlockCluster const &cluster, int pageY)
    : _data(cluster), _selected(_data.cleanBox), _pageY(pageY)
{
    this->setZValue(8);
}

/** Public **/

void SelectAreaBox::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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

QRectF SelectAreaBox::boundingRect() const
{
    const QRectF rect = _data.polygon.boundingRect();
    return rect.translated(0, _pageY);
}

bool SelectAreaBox::isSelected() const
{
    return _selected;
}

BlockCluster SelectAreaBox::getData() const
{
    BlockCluster cluster = _data;
    return cluster;
}

bool SelectAreaBox::isOnArea(QRectF area) const
{
    return area.contains(this->boundingRect());
}

/** Protected **/

void SelectAreaBox::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    _selected = !_selected;
    update();
}
