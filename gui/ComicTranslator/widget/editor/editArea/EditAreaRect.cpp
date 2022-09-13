#include "EditAreaRect.h"
#include "qwidget.h"
#include <QPainter>
#include <algorithm>
#include <QKeyEvent>
#include <QGraphicsLayout>
#include <QGraphicsSceneMouseEvent>
#include <QAbstractTextDocumentLayout>

using namespace std;

EditAreaRect::EditAreaRect(BlockCluster const &data, RectMode mode)
    : _mode(mode), _data(data), _text(_data.sentence)
{
    this->setFlag(ItemIsMovable, true);
    this->setFlag(ItemIsFocusable, true);

    if (mode == RectMode::EDIT_TRAN) {
        this->_text = _data.translation;
    }
    _textEdit = new QTextEdit(_text);
    _textEdit->setFont(_data.font);
    _textEdit->setTextColor(_data.color);
    _textEdit->setWordWrapMode(QTextOption::WordWrap);
    _textEdit->setStyleSheet("background-color: transparent; border: none");
    this->centerText();
    this->_textEdit->setReadOnly(true);
    this->_textEdit->setFixedWidth(this->boundingRect().width());
    this->setWidget(_textEdit);
    this->setZValue(8);
    this->setPos(data.box.x(), data.box.y());
}

QRectF EditAreaRect::boundingRect() const
{
    return QRectF(0, 0,
                  _data.box.width(), _data.box.height());
}

void EditAreaRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    QRectF rect = this->boundingRect();
    QColor borderColor = (_focusEdit) ? Qt::red
                                      : (hasFocus()
                                            ? Qt::blue
                                            : Qt::black);
    QPen pen(borderColor, 2 /* Line width */, Qt::DashDotLine);

    // Rectangle
    painter->setPen(pen);
    if (_mode == RectMode::EDIT_SENT) {
        painter->setBrush(QColor(255, 255, 255, 180)); // background color
    } else {
        painter->setBrush(Qt::transparent); // background color
    }
    painter->drawRect(rect);

    // Resize Triangle
    QPainterPath path;
    path.moveTo(rect.width() - RESIZE_CURSOR_SIZE, rect.height());
    path.lineTo(rect.width(), rect.height() - RESIZE_CURSOR_SIZE);
    path.lineTo(rect.width(), rect.height());
    path.lineTo(rect.width() - RESIZE_CURSOR_SIZE, rect.height());
    painter->setBrush(Qt::black); // background color
    painter->drawPath(path);

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

void EditAreaRect::setLineHeight(int percentage)
{
    QTextCursor textCursor = _textEdit->textCursor();
    QTextBlockFormat *newFormat = new QTextBlockFormat();

    textCursor.clearSelection();
    textCursor.select(QTextCursor::Document);
    newFormat->setLineHeight(percentage,
                            QTextBlockFormat::ProportionalHeight);
    newFormat->setAlignment(Qt::AlignCenter);
    textCursor.setBlockFormat(*newFormat);
    _data.line_height = percentage;
}

void EditAreaRect::setLineHeightAbs(int pixels)
{
    QTextCursor textCursor = _textEdit->textCursor();
    QTextBlockFormat *newFormat = new QTextBlockFormat();

    textCursor.clearSelection();
    textCursor.select(QTextCursor::Document);
    newFormat->setLineHeight(pixels,
                            QTextBlockFormat::MinimumHeight);
    newFormat->setAlignment(Qt::AlignCenter);
    textCursor.setBlockFormat(*newFormat);
}

BlockCluster const &EditAreaRect::getData()
{
    // Update position
    this->_data.box = QRect(x(), y(), _data.box.width(), _data.box.height());
    return _data;
}

/** Private **/

void EditAreaRect::resize(QPointF diff)
{
    QRect preview = _data.box.adjusted(0, 0, diff.x(), diff.y());

    if (preview.width() >= MIN_RECT_SIZE && preview.height() >= MIN_RECT_SIZE) {
        _data.box.adjust(0, 0, diff.x(), diff.y());

        this->_textEdit->setFixedWidth(this->_data.box.width());
        this->_textEdit->setFixedHeight(this->_data.box.height());
        this->centerText();
    }
}

void EditAreaRect::centerText()
{
    float realLineHeight = _textEdit->textCursor().blockFormat().lineHeight();
    float docHeight = _textEdit->document()->documentLayout()->documentSize().height();
    int rectHeight = _data.box.height();
    int lineHeight;
    int nbLine;

    if (realLineHeight == 0) {
        nbLine = _textEdit->document()->lineCount();
    } else {
        nbLine = docHeight / realLineHeight;
    }
    lineHeight = (float)rectHeight / (float)(nbLine + 1);
    this->setLineHeightAbs(lineHeight);
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
    this->_focusEdit = true;
    this->_textEdit->setReadOnly(false);
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
    this->_focusEdit = false;
    emit focusChanged(false, *this);
    update(); // refresh element
}

void EditAreaRect::focusInEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusInEvent(event);
    emit focusChanged(true, *this);
    update();
}

void EditAreaRect::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isResizing) {
        const QPointF pos = event->scenePos();
        const QPointF prevPos = event->lastScenePos();
        const QPointF shift = pos - prevPos;
        this->resize(shift);
    } else if (!_focusEdit) {
        QGraphicsItem::mouseMoveEvent(event);
    } else {
        QGraphicsProxyWidget::mouseMoveEvent(event);
    }
    // Mouse hover Resize (change cursor)
//    const QPointF pos = event->pos();
//    std::cerr << _data.box.width() - pos.x() << " " << _data.box.height() - pos.y() << "\n";
//    if (!_isResizing
//            && (_data.box.width() - pos.x()) < RESIZE_CURSOR_SIZE
//            && (_data.box.height() - pos.y()) < RESIZE_CURSOR_SIZE) {
//        this->setCursor(Qt::OpenHandCursor);
//        std::cerr << "set Cursor \n";
//    } else {
//        this->setCursor(Qt::ArrowCursor);
//    }
}

void EditAreaRect::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    const QPointF pos = event->pos();

    if ((_data.box.width() - pos.x()) < RESIZE_CURSOR_SIZE
            && (_data.box.height() - pos.y()) < RESIZE_CURSOR_SIZE) {
        // Resize
        this->_isResizing = true;
        this->setCursor(Qt::ClosedHandCursor);
    }
    if (!_focusEdit) {
        QGraphicsItem::mousePressEvent(event);
    } else {
       QGraphicsProxyWidget::mousePressEvent(event);
   }
}

void EditAreaRect::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    if (_isResizing) {
        _isResizing = false;
        this->setCursor(Qt::ArrowCursor);
    } else if (!_focusEdit) {
        QGraphicsItem::mouseReleaseEvent(event);
    } else {
        QGraphicsProxyWidget::mouseReleaseEvent(event);
    }
}

void EditAreaRect::keyPressEvent(QKeyEvent *event)
{
    QGraphicsProxyWidget::keyPressEvent(event);
    if (_focusEdit) {
        this->centerText();
    }
}
