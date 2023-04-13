#include "EditAreaRect.h"
#include "qwidget.h"
#include <QPainter>
#include <algorithm>
#include <QKeyEvent>
#include <QGraphicsLayout>
#include <QGraphicsSceneMouseEvent>
#include <QAbstractTextDocumentLayout>
#include <QScrollBar>
#include <QTextFrame>

using namespace std;

EditAreaRect::EditAreaRect(BlockCluster const &data, RectMode mode, int pageY)
    : QGraphicsProxyWidget(),
      _mode(mode),
      _data(data),
      _text((mode == RectMode::EDIT_SENT) ? _data.sentence : _data.translation),
      _pageY(pageY),
      _uuid(QUuid::createUuid())
{
    this->setFlag(ItemIsMovable, true);
    this->setFlag(ItemIsFocusable, true);

    _textEdit = new QTextEdit;
    _textEdit->setTextColor(_data.style.color);
    _textEdit->setText(_text);
    _textEdit->setFont(_data.style.font);
    _textEdit->setWordWrapMode(QTextOption::WordWrap);
    _textEdit->setStyleSheet("background-color: transparent; border: none");
    _textEdit->setContentsMargins(0, 0, 0, 0);
    _textEdit->verticalScrollBar()->hide();
    _textEdit->horizontalScrollBar()->hide();
    this->_textEdit->setReadOnly(true);
    this->_textEdit->setFixedWidth(data.polygon.boundingRect().width());
    this->_textEdit->setFixedHeight(data.polygon.boundingRect().height());
    this->setWidget(_textEdit);
    this->setZValue(8);
    this->setPos(data.polygon.boundingRect().x(), data.polygon.boundingRect().y() + pageY);
    this->_textEdit->setAlignment(Qt::AlignCenter);
    this->formatText();
    this->_textEdit->setPlaceholderText("text...");

    // Hide scroll bar
    _textEdit->horizontalScrollBar()->setStyleSheet("QScrollBar {height:0px;}");
    _textEdit->verticalScrollBar()->setStyleSheet("QScrollBar {width:0px;}");
}

QRectF EditAreaRect::boundingRect() const
{
    const QRectF rect = _data.polygon.boundingRect();
    return QRectF(0, 0, rect.width(), rect.height());
}

QUuid const &EditAreaRect::getUuid() const
{
    return _uuid;
}

void EditAreaRect::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    if (!_showBoxView) {
        // Show only text
        QGraphicsProxyWidget::paint(painter,option,widget);
        return;
    }
    QRectF rect = this->boundingRect();
    QColor borderColor = (_focusEdit) ? Qt::red
                                      : (_focus
                                            ? Qt::blue
                                            : Qt::black);
    QPen pen(borderColor, 2 /* Line width */, Qt::PenStyle::DashDotDotLine);

    // Rectangle
    painter->setPen(pen);
    if (_mode == RectMode::EDIT_SENT) {
        painter->setBrush(QColor(255, 255, 255, 222)); // background color
    } else {
        painter->setBrush(Qt::transparent); // background color
    }
    painter->drawRect(rect);

    QGraphicsProxyWidget::paint(painter,option, widget);

    // Resize Triangle
    QPainterPath path;
    path.moveTo(rect.width() - RESIZE_CURSOR_SIZE, rect.height());
    path.lineTo(rect.width(), rect.height() - RESIZE_CURSOR_SIZE);
    path.lineTo(rect.width(), rect.height());
    path.lineTo(rect.width() - RESIZE_CURSOR_SIZE, rect.height());
    painter->setBrush(Qt::black); // background color
    painter->drawPath(path);
}

void EditAreaRect::setStyle(RenderConfig const &style)
{
    this->_data.style = style;
    auto font = style.font;
    if (style.strokeWidth > 9 || !style.strokeWidth)
        throw std::invalid_argument("EditAreaRect::setStyle Invalid bold value");
    font.setWeight(weightChoices[style.strokeWidth - 1]); // Range: 100-900
    this->setFont(font);
    this->setTextColor(style.color);
    this->setLineHeightAbs(_data.style.lineHeight);
    this->formatText();
}

void EditAreaRect::setFont(const QFont &font)
{
    _data.style.font = font;
    _textEdit->setFont(_data.style.font);
}

void EditAreaRect::setTextColor(QColor const &color)
{
    _data.style.color = color;
    _textEdit->setTextColor(color);
    // refresh Text format
    QString const &txt = _textEdit->toPlainText();
    _textEdit->setText(txt);
    this->formatText();
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
    _data.style.lineHeight = percentage;
}

void EditAreaRect::setLineHeightAbs(int pixels)
{
    QTextCursor textCursor = _textEdit->textCursor();
    QTextBlockFormat newFormat = textCursor.blockFormat();

    textCursor.clearSelection();
    textCursor.select(QTextCursor::Document);
    newFormat.setLineHeight(pixels,
                            QTextBlockFormat::MinimumHeight);
    newFormat.setAlignment(Qt::AlignCenter);
    textCursor.setBlockFormat(newFormat);
}

BlockCluster EditAreaRect::getData()
{
    BlockCluster cluster = _data;
    // Update position
    QPointF diff = this->pos() - this->_data.polygon.boundingRect().topLeft();
    cluster.polygon.translate(diff.x(), diff.y());
    cluster.polygon.translate(0, -_pageY); // Convert coord => relative to page top left corner
    if (_mode == RectMode::EDIT_SENT) {
        cluster.sentence = _textEdit->toPlainText();
    } else {
        cluster.translation = _textEdit->toPlainText();
    }
    return cluster;
}

bool EditAreaRect::isOnArea(QRectF const &area) const
{
    const QRectF &rect = this->boundingRect();
    return rect.translated(this->pos()).intersects(area);
}

void EditAreaRect::removeFocus()
{
    this->setFlag(ItemIsMovable, true);
    this->_textEdit->setReadOnly(true);
    this->_focusEdit = false;
    this->_focus = false;
    this->clearFocus();
    update(); // refresh element
}

/** Private **/

void EditAreaRect::resize(QPointF diff)
{
    const QRectF &rect = this->boundingRect();
    const QRectF &currentRect = rect.adjusted(0, 0, diff.x(), diff.y()); // Rect after resize

    if (currentRect.width() >= MIN_RECT_SIZE && currentRect.height() >= MIN_RECT_SIZE) {
        QPointF scale(
                    (rect.width()) ? currentRect.width() / rect.width() : 1,
                    (rect.height()) ? currentRect.height() / rect.height() : 1);
        _data.polygon = QTransform().scale(scale.x(), scale.y()).map(_data.polygon);

        this->_textEdit->setFixedWidth(currentRect.width());
        this->_textEdit->setFixedHeight(currentRect.height());
        this->formatText();
    }
}

void EditAreaRect::showEvent(QShowEvent *event)
{
    QGraphicsProxyWidget::showEvent(event);
    this->formatText();
}

int EditAreaRect::computeOptimalFontSize(int *heightMargin, QString const &text) const
{
    const int MAX_FONTSIZE = 100;
    int fontSize = 1;
    const QRegularExpression regExpr("[\\s\\t]");
    const QSize &rect = _textEdit->size();
    int marginHeight = 0;
    const QStringList &wordList = text.split(regExpr, Qt::SkipEmptyParts);

    if (_data.style.strokeWidth > 9 || !_data.style.strokeWidth)
        throw std::runtime_error("EditAreaRect::formatText invalid font weight");
    // Compute size of text for a font size => check it fit.
    // Then computer bigger size. Stop when the text don't fit anymore.
    while (fontSize < MAX_FONTSIZE/* Computing time limit */) {
        QFont font = QFont(_data.style.font.families(), fontSize);
        font.setWeight(weightChoices[_data.style.strokeWidth - 1]);
        const QFontMetrics fm(font);
        const int spaceWidth = fm.horizontalAdvance(" ");
        int fontHeight = fm.height();
        unsigned int lineWidth = 0;
        unsigned int nbLine = 1;

        if ((unsigned int)fontHeight < _data.style.lineHeight) {
            fontHeight = _data.style.lineHeight;
        }
        for (QString const &word : wordList) {
            bool containLineBreak = word.contains('\n');
            int wordWidth = fm.horizontalAdvance(word);
            if (wordWidth >= rect.width()) {
                *heightMargin = marginHeight;
                return fontSize - 1; // If one word is larger than the rect size.
            } else if (lineWidth + wordWidth + spaceWidth > rect.width() || containLineBreak) {
                nbLine++;
                lineWidth = wordWidth + spaceWidth;
            } else {
                lineWidth += wordWidth + spaceWidth;
            }
        }
        const unsigned int blockHeight = nbLine * fontHeight;
        if (blockHeight >= (unsigned int)rect.height() - 10 /* error margin */) {
            *heightMargin = 0; // No need, the text will take the full height in that case
            return fontSize - 1; // Font size too large, select the previous one.
        }
        marginHeight = rect.height() - blockHeight;
        fontSize++;
    }
    return MAX_FONTSIZE;
}

void EditAreaRect::formatText()
{
    int heightMargin = 0;

    // Fill rect by modifying the font size
    QString text = _textEdit->toPlainText();
    if (text.isEmpty()) {
        text = _textEdit->placeholderText();
    }
    int fontSize = this->computeOptimalFontSize(&heightMargin, text);
    _data.style.font = QFont(_data.style.font.families(), fontSize);
    if (_data.style.strokeWidth > 9 || !_data.style.strokeWidth)
        throw std::runtime_error("EditAreaRect::formatText invalid font weight");
    _data.style.font.setWeight(weightChoices[_data.style.strokeWidth - 1]);
    this->_textEdit->setFont(_data.style.font);

    // Center text vertically by adding top margin.
    const int topMargin = heightMargin / 2;
    auto format = this->_textEdit->document()->rootFrame()->frameFormat();
    format.setTopMargin(topMargin);
    this->_textEdit->document()->rootFrame()->setFrameFormat(format);

//     // Center text by modifying the ligne height
//     float realLineHeight = _textEdit->textCursor().blockFormat().lineHeight();
//     float docHeight = _textEdit->document()->documentLayout()->documentSize().height();
//     int rectHeight = this->boundingRect().height();
//     int nbLine;

//     if (realLineHeight == 0) {
//         nbLine = _textEdit->document()->lineCount();
//     } else {
//         nbLine = docHeight / realLineHeight;
//     }
//     int lineHeight = (float)rectHeight / (float)(nbLine + 1);
//     this->setLineHeightAbs(lineHeight); // _data.style.lineHeight
}

void EditAreaRect::enableBoxView(bool enable)
{
    this->_showBoxView = enable;
    this->update();
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

void EditAreaRect::focusOutEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusOutEvent(event);
    emit focusChanged(false, this);
}

void EditAreaRect::focusInEvent(QFocusEvent *event)
{
    QGraphicsProxyWidget::focusInEvent(event);
    this->_focus = true;
    emit focusChanged(true, this);
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

    if ((boundingRect().width() - pos.x()) < RESIZE_CURSOR_SIZE
            && (boundingRect().height() - pos.y()) < RESIZE_CURSOR_SIZE) {
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
        this->formatText();
    }
}
