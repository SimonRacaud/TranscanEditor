#include "EditAreaRect.h"
#include "qwidget.h"
#include <QPainter>
#include <algorithm>
#include <QKeyEvent>
#include <QGraphicsLayout>

using namespace std;

EditAreaRect::EditAreaRect(BlockCluster const &data) : _data(data), _text(_data.sentence)
{
    this->setFlag(ItemIsMovable, true);
    this->setFlag(ItemIsFocusable, true);

    _textEdit = new QTextEdit(_text);
    _textEdit->setAlignment(Qt::AlignCenter);
    _textEdit->setFont(_data.font);
    _textEdit->setTextColor(_data.color);
    this->setLineHeight(_data.line_height);
    _textEdit->setWordWrapMode(QTextOption::WordWrap);
    this->_textEdit->setReadOnly(true);
    this->_textEdit->setFixedWidth(this->boundingRect().width());
    setWidget(_textEdit);
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

void EditAreaRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = this->boundingRect();
    QColor borderColor = (_focus) ? Qt::red : Qt::black;
    QPen pen(borderColor, 2 /* Line width */, Qt::DashDotLine);

    // Rectangle
    painter->setPen(pen);
    painter->setBrush(Qt::white); // background color
    painter->drawRect(rect);

    QGraphicsProxyWidget::paint(painter,option,widget);
}

void EditAreaRect::setFont(QFont &font)
{
    _data.font = font;
    _textEdit->setFont(_data.font);
}

void EditAreaRect::setTextColor(QColor color)
{
    _data.color = color;
    _textEdit->setTextColor(color);
}

void EditAreaRect::setLineHeight(int size)
{
    QTextCursor textCursor = _textEdit->textCursor();
    QTextBlockFormat *newFormat = new QTextBlockFormat();

    textCursor.clearSelection();
    textCursor.select(QTextCursor::Document);
    newFormat->setLineHeight(size,
                            QTextBlockFormat::ProportionalHeight);
    newFormat->setAlignment(Qt::AlignCenter);
    textCursor.setBlockFormat(*newFormat);
    _data.line_height = size;
}

/** Protected **/

/**
 * @brief Start editing
 * @param event
 */
void EditAreaRect::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsObject::mouseDoubleClickEvent(event);
    this->setFlag(ItemIsMovable, false);
    this->_focus = true;
    this->_textEdit->setReadOnly(false);
    emit focusChanged(_focus, *this);
}

/**
 * @brief Stop editing
 * @param event
 */
void EditAreaRect::focusOutEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusOutEvent(event);
    this->setFlag(ItemIsMovable, true);
    this->_textEdit->setReadOnly(true);
    this->_focus = false;
    emit focusChanged(_focus, *this);
}

void EditAreaRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (!_focus) {
        QGraphicsItem::mouseMoveEvent(event);
    } else {
        QGraphicsProxyWidget::mouseMoveEvent(event);
    }
}

void EditAreaRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
   if (!_focus) {
        QGraphicsItem::mousePressEvent(event);
   } else {
       QGraphicsProxyWidget::mousePressEvent(event);
   }
}

void EditAreaRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (!_focus) {
        QGraphicsItem::mouseReleaseEvent(event);
    } else {
        QGraphicsProxyWidget::mouseReleaseEvent(event);
    }
}
