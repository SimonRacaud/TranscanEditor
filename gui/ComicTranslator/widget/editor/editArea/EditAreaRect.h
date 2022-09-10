#ifndef RECTAREAITEM_H
#define RECTAREAITEM_H

#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <iostream>
#include "include/model/OCRPage.h"

class EditAreaRect : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    EditAreaRect(BlockCluster const &data);

    QRectF boundingRect() const override;

    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) override;

    void setFont(QFont &font);
    void setTextColor(QColor color);
    /**
     * @brief setLineHeight
     * @param size (percentage)
     */
    void setLineHeight(int size);

signals:
    void focusChanged(bool state, EditAreaRect &rect);

private slots:


protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    BlockCluster _data;
    QString &_text;

    QGraphicsProxyWidget *_proxy{nullptr};
    QTextEdit *_textEdit{nullptr};
    bool _focus{false};
};

#endif // RECTAREAITEM_H
