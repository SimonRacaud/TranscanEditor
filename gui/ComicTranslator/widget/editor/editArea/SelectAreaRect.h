#ifndef SELECTAREARECT_H
#define SELECTAREARECT_H

#include <QGraphicsObject>
#include <QObject>

#include "include/model/OCRPage.h"

class SelectAreaRect : public QGraphicsObject
{
    Q_OBJECT
public:
    SelectAreaRect(BlockCluster const &data);

    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) override;

    QRectF boundingRect() const override;

    bool isSelected() const;

    BlockCluster const &getData() const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    BlockCluster _data;
    bool &_selected;
};

#endif // SELECTAREARECT_H
