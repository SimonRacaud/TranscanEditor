#ifndef FONTSELECT_H
#define FONTSELECT_H

#include <QLabel>

///

#include <QListView>

class FontListView : public QListView
{
    Q_OBJECT
public:
    FontListView() = default;

    virtual void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected) override;

signals:
    void sigSelectFont(QString const &family);
};

///

#include <QTabWidget>

class FontSelectList : public QTabWidget
{
    Q_OBJECT
public:
    explicit FontSelectList(QWidget *parent);

signals:
    void sigSelectFont(QString const &family);
    void sigClosed();

private:
    void selectFont(QString const &family);

    virtual void closeEvent(QCloseEvent *event);

private:
    FontListView *_regularFontList;
    FontListView *_sfxFontList;
    FontListView *_localFontList;
};

///

#include <QtUiPlugin/customwidget.h>

class FontSelect : public QWidget, public QDesignerCustomWidgetInterface
{
    Q_OBJECT
public:
    explicit FontSelect(QWidget *parent = nullptr);

    QFont currentFont() const;
    void setCurrentFont(QFont const &font);

    /* Qt Design */
    virtual QString name() const;
    virtual QString group() const;
    virtual QString toolTip() const;
    virtual QString whatsThis() const;
    virtual QString includeFile() const ;
    virtual QIcon icon() const;
    virtual bool isContainer() const;
    virtual QWidget *createWidget(QWidget *parent);

public slots:
    void open();
    void close();

private:
    virtual void mousePressEvent(QMouseEvent *event);
    /**
     * @brief paintEvent : used to draw background
     * @param event
     */
    virtual void paintEvent(QPaintEvent* event);

private slots:
    void selectFont(QString const &family);

signals:
    void currentIndexChanged();

private:
    QLabel *_selectedFont;
    QLabel *_openIndicator;
    bool _isOpen{false};
    QFont _currFont;
    FontSelectList *_fontList{nullptr};
};

///

#include <QStringListModel>

class FontSelectListModel : public QStringListModel
{
public:
    FontSelectListModel(QWidget *parent);

    virtual void setStringList(const QStringList &strings);
    virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

private:
    std::map<int, QFont> _indexToFont;
};

#endif // FONTSELECT_H
