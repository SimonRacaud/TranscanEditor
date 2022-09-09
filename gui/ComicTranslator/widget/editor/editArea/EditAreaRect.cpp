#include "EditAreaRect.h"
#include <QPainter>
#include <algorithm>

using namespace std;

/**
 * @brief The EditAreaRect class
 *
 * - Text area, dotted line border, (plain background)
 *      - can be resized
 *      - can be removed
 *      - can be selected
 *      - style: set text color, font family, font size, line height
 *
 * - Selection area, dotted line border, (50% plain background or none)
 *      - can be enabled/disabled (hide the text or not)
 *
 */

EditAreaRect::EditAreaRect(BlockCluster const &data) : _data(data)
{
    // this->setFlag(ItemIsMovable);
    // https://doc.qt.io/qt-5/qgraphicsitem.html
    // https://doc.qt.io/qt-5/qgraphicswidget.html

    // https://forum.qt.io/topic/85649/qgraphicsitem-border-style/4
    // https://www.bogotobogo.com/Qt/Qt5_QGraphicsView_QGraphicsScene_QGraphicsItems.php
}

QRectF EditAreaRect::boundingRect() const
{
    int minX = INT_MAX, maxX = 0, minY = INT_MAX, maxY = 0;

    for (int i = 0; i < 4; i++) {
        minX = min(minX, _data.polygon[i][0]);
        maxX = max(maxX, _data.polygon[i][0]);
        minY = min(minY, _data.polygon[i][1]);
        maxY = max(maxY, _data.polygon[i][1]);
    }
    return QRectF(minX, minY, maxX - minX, maxY - minY);
}

QPolygon &EditAreaRect::boundingPoly()
{
    for (int i = 0; i < 4; i++) {
        _polygon << QPoint(_data.polygon[i][0], _data.polygon[i][1]);
    }
    return _polygon;
}

void EditAreaRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = this->boundingRect();
    QPolygon &poly = this->boundingPoly();
    QPen pen(Qt::black, 2 /* Line width */, Qt::DashDotLine);

    painter->setPen(pen);
    painter->drawPolygon(poly);
    painter->drawText(rect, _data.sentence, QTextOption(Qt::AlignCenter));
}
