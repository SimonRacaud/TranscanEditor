#ifndef RECTAREAITEM_H
#define RECTAREAITEM_H

#include <QGraphicsItem>
#include "include/model/OCRPage.h"

class EditAreaRect : public QGraphicsObject
{
    Q_OBJECT
public:
    EditAreaRect(BlockCluster const &data);

    QRectF boundingRect() const;

    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget);

private:
    QPolygon &boundingPoly();

private:
    BlockCluster _data;

    QPolygon _polygon;
};

#endif // RECTAREAITEM_H
