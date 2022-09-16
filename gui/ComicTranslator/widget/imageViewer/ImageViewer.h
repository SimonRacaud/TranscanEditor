#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <iostream>
#include <QGraphicsPixmapItem>

#include "include/environment.h"
#include "include/model/OCRPage.h"
#include "PageGraphicsView.h"

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    explicit ImageViewer(QWidget *parent = nullptr);

    virtual void setPages(std::vector<OCRPage> const &pages);

    void setLoadingState(bool enable);

    /**
     * @brief scale : zoom in or out
     * @param scale
     */
    void scale(float scale);

protected slots:
    void refreshImageListSlot();

public slots:
    void setVerticalScrollPosition(int value);
    void setHorizontalScrollPosition(int value);

signals:
    void verticalScrollValueChanged(int value);
    void horizontalScrollValueChanged(int value);

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

private:
    // Loading icon
    QMovie *_loadingImg;
    QLabel *_loadingWidget;
    bool _loading{false};

    float _scale{1.0}; // Scale of the images
    QList<QGraphicsPixmapItem *> _pageItems; // Buffer of pages
    int _timePreviousResize{0};

    size_t _imageWidth{0};

protected:
    std::vector<OCRPage> _pages;
    PageGraphicsView *_view;
    QGraphicsScene *_scene;
    QList<QPixmap> _pixmapList; // Original images buffer
};

#endif // IMAGEVIEWER_H
