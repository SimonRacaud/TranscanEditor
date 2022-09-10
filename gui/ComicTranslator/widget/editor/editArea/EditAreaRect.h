#ifndef RECTAREAITEM_H
#define RECTAREAITEM_H

#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <iostream>
#include "include/model/OCRPage.h"

#define MIN_RECT_SIZE 40
#define RESIZE_CURSOR_SIZE 15

enum class RectMode {
    EDIT_SENT, // edit sentence
    EDIT_TRAN, // edit translation
};

class EditAreaRect : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    EditAreaRect(BlockCluster const &data, RectMode mode);

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

private:
    /**
     * @brief resize
     * @param diff (size incremented by diff)
     */
    void resize(QPointF diff);

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

private:
    RectMode _mode;
    BlockCluster _data;
    QString &_text;

    QGraphicsProxyWidget *_proxy{nullptr};
    QTextEdit *_textEdit{nullptr};
    bool _focusEdit{false};
    bool _focus{false};
    bool _isResizing{false};
};

#endif // RECTAREAITEM_H
