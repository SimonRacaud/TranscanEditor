#ifndef SELECTAREABOX_H
#define SELECTAREABOX_H

#include <QGraphicsObject>
#include <QObject>

#include "model/Page.h"

/**
 * @brief The SelectAreaBox class : Used to select areas on a page (clean process)
 * This widget is only clickable (selected or ignored)
 */
class SelectAreaBox : public QGraphicsObject
{
    Q_OBJECT
public:
    SelectAreaBox(BlockCluster const &data, int pageY);

    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) override;

    QRectF boundingRect() const override;

    bool isSelected() const;

    BlockCluster getData() const;

    /**
     * @brief isOnSpace True if the rect is located on the target area
     * @param index Index of the page
     * @return
     */
    bool isOnArea(QRectF area) const;

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;

private:
    BlockCluster _data;
    bool &_selected;
    const int _pageY;
};

#endif // SELECTAREABOX_H
