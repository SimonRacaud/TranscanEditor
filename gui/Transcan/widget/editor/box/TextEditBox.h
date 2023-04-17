#ifndef TEXT_EDIT_BOX_H
#define TEXT_EDIT_BOX_H

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

class TextEditBox : public QGraphicsProxyWidget
{
    Q_OBJECT
public:
    /**
     * @brief TextEditBox
     * @param data
     * @param mode
     * @param pageY : Position on Y axis of the page in the scene
     */
    TextEditBox(BlockCluster const &data, RectMode mode, int pageY);

    QRectF boundingRect() const override;

    virtual void paint(
            QPainter *painter,
            const QStyleOptionGraphicsItem *option,
            QWidget *widget) override;

    void setFont(const QFont &font);
    void setTextColor(QColor const &color);

    // Text line height
    void setLineHeight(int percentage);
    void setLineHeightAbs(int pixels);

    /**
     * @brief setStyle : Set text style
     * @param style
     */
    void setStyle(RenderConfig const &style);

    /**
     * @brief getData
     * @return
     */
    BlockCluster getData();

    /**
     * @brief isOnSpace True if the rect is located on the target area
     * @param index Index of the page
     * @return
     */
    bool isOnArea(QRectF const &area) const;

    /**
     * @brief removeFocus : If the element is focused, remove it
     */
    void removeFocus();

    /**
     * @brief getUuid : Return object unique ID
     * @return
     */
    QUuid const &getUuid() const;

    /**
     * @brief enableBoxView : Show or Hide box border and corner triangle
     * @param enable
     */
    void enableBoxView(bool enable = true);

signals:
    void focusChanged(bool state, TextEditBox *rect);

private:
    /**
     * @brief resize
     * @param diff (size incremented by diff)
     */
    void resize(QPointF diff);

    virtual void showEvent(QShowEvent *event) override;

    /**
     * @brief computeOptimalFontSize : Computer the maximum font size for the
     *  _text to fit inside the boundingRect.
     * @return
     */
    int computeOptimalFontSize(int *heightMargin, QString const &text) const;

private slots:
    /**
     * @brief centerText : Center text vertically by modifying the line height.
     */
    void formatText();

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
    int _pageY;
    QUuid _uuid;

    QGraphicsProxyWidget *_proxy{nullptr};
    QTextEdit *_textEdit{nullptr};
    bool _focusEdit{false};
    bool _focus{false};
    bool _isResizing{false};
    bool _showBoxView{true};
};

#endif // TEXT_EDIT_BOX_H
