#ifndef RECTAREAITEM_H
#define RECTAREAITEM_H

#include <QGraphicsProxyWidget>
#include <QTextEdit>
#include <iostream>

#include "model/Page.h"

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

    // Text line height
    void setLineHeight(int percentage);
    void setLineHeightAbs(int pixels);

    BlockCluster getData();

    /**
     * @brief isOnSpace True if the rect is located on the target area
     * @param index Index of the page
     * @return
     */
    bool isOnArea(QRectF const &area) const;

signals:
    void focusChanged(bool state, EditAreaRect &rect);

private:
    /**
     * @brief resize
     * @param diff (size incremented by diff)
     */
    void resize(QPointF diff);

    void centerText();    

protected:
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void focusOutEvent(QFocusEvent *event) override;
    virtual void focusInEvent(QFocusEvent *event) override;

    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event) override;

    virtual void keyPressEvent(QKeyEvent *event) override;

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
