#ifndef VIEWERWINDOW_H
#define VIEWERWINDOW_H

#include <iostream>

#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QSpacerItem>
#include <QScrollArea>
#include <QImage>
#include <QFileDialog>
#include <QListWidget>
#include <QListWidgetItem>
#include <QScrollBar>
#include <QEvent>

class ViewerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ViewerWindow(QWidget *parent = nullptr, bool integratedMode = false);
    virtual ~ViewerWindow() = default;

    void loadDirectory(QString const &dirPath);

    QScrollBar *getVerticalScroll() const;
    QScrollBar *getHorizontalScroll() const;

    void scale(float scale);

private slots:
    void openFolderSlot();
    void goBackSlot();

public slots:
    void setVerticalScrollPosition(int value);
    void setHorizontalScrollPosition(int value);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    void clearImageList();
    void resizeImages(int const width);

public:
    static const QList<QString> SUPPORTED_EXTENSION;

private:
    QPushButton *_backButton{nullptr};
    QPushButton *_openFolderButton{nullptr};
    QLabel *_title{nullptr};

    QScrollArea *_scrollArea;
    QVBoxLayout *_imageList;
    QList<QImage *> _listImage;
    QList<QLabel *> _listImgLabel;

    int _timePreviousResize = 0;
    bool _integratedMode;
    unsigned short _zoom{100}; // Percentage
signals:

};

#endif // VIEWERWINDOW_H
