#include "SelectAreaRect.h"

#include <QPainter>

SelectAreaRect::SelectAreaRect(BlockCluster const &cluster, int pageY)
    : _data(cluster), _selected(_data.cleanBox), _pageY(pageY)
{
    this->setZValue(8);
}

/** Public **/

void SelectAreaRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *, QWidget *)
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
    const QRectF rect = _data.polygon.boundingRect();
    return rect.translated(0, _pageY);
}

bool SelectAreaRect::isSelected() const
{
    return _selected;
}

BlockCluster SelectAreaRect::getData() const
{
    BlockCluster cluster = _data;
    cluster.polygon.translate(0, _pageY);
    return cluster;
}

bool SelectAreaRect::isOnArea(QRectF area) const
{
    return area.contains(this->boundingRect());
}

/** Protected **/

void SelectAreaRect::mousePressEvent(QGraphicsSceneMouseEvent *)
{
    _selected = !_selected;
    update();
}
