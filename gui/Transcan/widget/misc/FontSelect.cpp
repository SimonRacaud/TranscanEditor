#include "FontSelect.h"
#include <QHBoxLayout>

#include <QStyleOption>
#include <QPainter>

#include "window/MainWindow.h"
#include "utility/FontManager.h"

extern MainWindow *mainWindow;

static const QString ClosedChar = "▼";
static const QString OpenChar = "▲";

FontSelect::FontSelect(QWidget *parent)
    : QWidget(parent)
{
    _selectedFont = new QLabel("");
    _openIndicator = new QLabel(ClosedChar);

    auto *lay = new QHBoxLayout;
    lay->addWidget(_selectedFont, 0, Qt::AlignmentFlag::AlignLeft);
    lay->addWidget(_openIndicator, 0, Qt::AlignmentFlag::AlignRight);
    this->setLayout(lay);
    this->setObjectName("FontSelectWidget");
    this->_openIndicator->setObjectName("FontSelectWidgetOpenChar");
}

void FontSelect::open()
{
    if (_fontList == nullptr) {
        this->_fontList = new FontSelectList(this);
        connect(_fontList, &FontSelectList::sigSelectFont, this, &FontSelect::selectFont);
        connect(_fontList, &FontSelectList::sigClosed, this, &FontSelect::close);
    } else {
        this->_fontList->show();
    }
    QPoint globalCursorPos = QCursor::pos();
    _fontList->move(globalCursorPos);
    //
    this->_openIndicator->setText(OpenChar);
    _isOpen = true;
}

void FontSelect::close()
{
    if (_fontList) {
        this->_fontList->close();
    }
    this->_openIndicator->setText(ClosedChar);
    _isOpen = false;
}

QFont FontSelect::currentFont() const
{
    return this->_currFont;
}

void FontSelect::setCurrentFont(QFont const &font)
{
    this->_currFont = font;
    this->_selectedFont->setText(font.family());
}

void FontSelect::paintEvent(QPaintEvent* event)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);

    QWidget::paintEvent(event);
}

/** QT Designer **/

QString FontSelect::name() const
{
    return "FontSelect";
}
QString FontSelect::group() const
{
    return "Custom";
}
QString FontSelect::toolTip() const
{
    return "Font family selector for comic books";
}
QString FontSelect::whatsThis() const
{
   return "Select a font family among Regular, SFX or other local fonts";
}
QString FontSelect::includeFile() const
{
    return "widget/misc/FontSelect.h";
}
QIcon FontSelect::icon() const
{
    return QIcon();
}
bool FontSelect::isContainer() const
{
    return false;
}
QWidget *FontSelect::createWidget(QWidget *parent)
{
    return new FontSelect(parent);
}

/** PRIVATE **/

void FontSelect::mousePressEvent(QMouseEvent *)
{
    if (_isOpen) {
        this->close();
    } else {
        this->open();
    }
}

void FontSelect::selectFont(QString const &family)
{
    this->setCurrentFont(QFont(family));
    this->close();
    emit this->currentIndexChanged();
}

/////////////////////////////////////////////////////:
/// FontSelectList

FontSelectList::FontSelectList(QWidget *)
    : QTabWidget()
{
    this->setObjectName("FontSelectListWidget");
    _regularFontList = new FontListView();
    _regularFontList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _sfxFontList = new FontListView();
    _sfxFontList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    _localFontList = new FontListView();
    _localFontList->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->addTab(_regularFontList, "Regular");
    this->addTab(_sfxFontList, "SFX");
    this->addTab(_localFontList, "Computer");

    const FontManager &fm = FontManager::getInstance();
    // Make data
    QStringList listLocalFont = fm.families();
    QStringList listRegFont = fm.regularFamilies();
    QStringList listSfxFont = fm.sfxFamilies();
    // Populate our model
    auto *modelLocal = new FontSelectListModel(this);
    modelLocal->setStringList(listLocalFont);
    _localFontList->setModel(modelLocal);

    auto *modelReg = new FontSelectListModel(this);
    modelReg->setStringList(listRegFont);
    _regularFontList->setModel(modelReg);

    auto *modelSfx= new FontSelectListModel(this);
    modelSfx->setStringList(listSfxFont);
    _sfxFontList->setModel(modelSfx);
    //
    this->resize(300, 400);
    this->raise();
    this->show();

    // Events
    connect(_regularFontList, &FontListView::sigSelectFont, this, &FontSelectList::selectFont);
    connect(_sfxFontList, &FontListView::sigSelectFont, this, &FontSelectList::selectFont);
    connect(_localFontList, &FontListView::sigSelectFont, this, &FontSelectList::selectFont);
}

void FontSelectList::closeEvent(QCloseEvent *)
{
    emit this->sigClosed();
}

void FontSelectList::selectFont(QString const &family)
{
    this->hide();
    emit this->sigSelectFont(family);
}

/////////////////////////////////////////////////////:
/// FontListView

void FontListView::selectionChanged(const QItemSelection &,
                                    const QItemSelection &)
{
       QModelIndex mi = this->currentIndex();
       QString const &family = mi.data().toString();
       emit this->sigSelectFont(family);
}

/////////////////////////////////////////////////////:
/// FontSelectListModel

FontSelectListModel::FontSelectListModel(QWidget *parent)
    : QStringListModel(parent)
{
}

bool FontSelectListModel::setData(const QModelIndex &index,
                                  const QVariant &value,
                                  int role)
{
    if (role == Qt::ItemDataRole::DisplayRole) {
        QString const &fontName = value.toString();
        QFont f(fontName);
        if (!f.exactMatch()) {
            qDebug() << "Error: FontSelectListModel::setData, font " << fontName << " not found.";
        }
        this->_indexToFont[index.row()] = f;
        return true;
    }
    return QStringListModel::setData(index, value, role);
}

void FontSelectListModel::setStringList(const QStringList &strings)
{
    size_t idx = 0;
    for (QString const &family : strings) {
        QFont f(family);
        if (!f.exactMatch()) {
            qDebug() << "Error: FontSelectListModel::setStringList, font " << family << " not found.";
        }
        this->_indexToFont[idx] = f;
        idx++;
    }
    QStringListModel::setStringList(strings);
}

QVariant FontSelectListModel::data(const QModelIndex &index, int role) const
{
    if (role == Qt::ItemDataRole::FontRole) {
        auto itr = _indexToFont.find(index.row());
        if (itr != _indexToFont.end())
            return itr->second; // Return found QFont
    }
    return QStringListModel::data(index, role);
}
