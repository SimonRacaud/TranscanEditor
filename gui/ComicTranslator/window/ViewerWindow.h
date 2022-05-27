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
#include <QEvent>

#include <QDebug>

// TEMP CONFIG
#define WIN_INIT_WIDTH 600 // px
#define WIN_INIT_HEIGHT 800 // px
#define TITLE_MAX_SIZE 500 // character
#define TIME_REFRESH_RESIZE 5 // ms

#define GUI_WIDTH_SHIFT 40 // px
//

class ViewerWindow : public QWidget
{
    Q_OBJECT
public:
    explicit ViewerWindow(QWidget *parent = nullptr);
    virtual ~ViewerWindow() = default;

    void loadDirectory(QString const &dirPath);

private slots:
    void openFolderSlot();
    void goBackSlot();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    void clearImageList();
    void resizeImages(int const width);

public:
    static const QList<QString> SUPPORTED_EXTENSION;

private:
    QPushButton *_backButton;
    QPushButton *_openFolderButton;
    QLabel *_title;
    QScrollArea *_scrollArea;
    QVBoxLayout *_imageList;
    QList<QImage *> _listImage;
    QList<QLabel *> _listImgLabel;

    int _timePreviousResize = 0;
signals:

};

#endif // VIEWERWINDOW_H
