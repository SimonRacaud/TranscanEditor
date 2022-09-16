#include "ImageViewer.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QMovie>
#include <QKeyEvent>

using namespace std;

ImageViewer::ImageViewer(QWidget *parent) : QWidget{parent}
{
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_EDITA_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    //
    QHBoxLayout *rootLayout = new QHBoxLayout;
    // Editor - scene
    this->_view = new PageGraphicsView();
    this->_scene = new QGraphicsScene();
    this->_view->setScene(_scene);
    this->_view->setRenderHint(QPainter::Antialiasing);
    connect(_view, &PageGraphicsView::resizedSignal, this, &ImageViewer::refreshImageListSlot);
    //
    rootLayout->addWidget(_view);
    rootLayout->setSpacing(0);
    rootLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(rootLayout);
    // Synchronize scroll Editor and Viewer
    connect(this->_view->verticalScrollBar(), &QScrollBar::valueChanged, this, [this](int value){
        emit this->verticalScrollValueChanged(value);
    });
    connect(this->_view->horizontalScrollBar(), &QScrollBar::valueChanged, this, [this](int value){
        emit this->horizontalScrollValueChanged(value);
    });
    // Loading icon
    this->_loadingWidget = new QLabel(this);
    this->_loadingImg = new QMovie();
    _loadingImg->setFileName(IMG_LOADING);
    this->_loadingWidget->setMovie(_loadingImg);
    _loadingImg->setScaledSize(QSize(LOADING_ICON_SIZE, LOADING_ICON_SIZE));
    _loadingWidget->resize(LOADING_ICON_SIZE, LOADING_ICON_SIZE);
}

/** PUBLIC **/
#include <QtGlobal>

void ImageViewer::setPages(vector<OCRPage> const &pages)
{
    this->_pages = pages;
    size_t offsetY = 0;

    this->_imageWidth = 0;
    for (const OCRPage &page : pages) {
        QPixmap img(page.imagePath); // TODO : get the right image to display
        this->_pixmapList.append(img);
        auto *item = new QGraphicsPixmapItem(img);
        item->setPos(0, offsetY);
        this->_scene->addItem(item);
        this->_pageItems.push_back(item);
        offsetY += img.height();
        this->_imageWidth = qMax(_imageWidth, (size_t)img.width());
    }
}

void ImageViewer::setLoadingState(bool enable)
{
    this->_loading = enable;
    if (enable) {
        _loadingWidget->move(this->width() / 2 - _loadingWidget->width() / 2,
                             this->height() / 2 - _loadingWidget->height() / 2);
        _loadingImg->start();
    } else {
        this->_loadingImg->stop();
    }
}

void ImageViewer::scale(float scale)
{
    this->_view->scale(scale, scale);
}

/** INTERNAL **/

void ImageViewer::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Resize Image List width
    int now = QTime::currentTime().msecsSinceStartOfDay();
    if ((now - _timePreviousResize) > EDITOR_TIME_REFRESH_RESIZE) {
        if (_loading) {
            _loadingWidget->move(this->width() / 2 - _loadingWidget->width() / 2,
                                 this->height() / 2 - _loadingWidget->height() / 2);
        }
        this->_timePreviousResize = now;
    }
}

/** SLOTS **/

void ImageViewer::refreshImageListSlot()
{
    int viewWidth = this->_view->width() /* Widget borders */ - 4;
    qreal offsetY = 0;

    if (this->_view->verticalScrollBar()->isEnabled()) {
        viewWidth -= this->_view->verticalScrollBar()->width();
    }
    float scale = (float)viewWidth / (float)_imageWidth;
    // Scale widget for the image to fix the view width:
    this->scale(scale);
    // Update displayed images width:
    this->_imageWidth = _imageWidth * scale;
}

void ImageViewer::setVerticalScrollPosition(int value)
{
    this->_view->verticalScrollBar()->setValue(value);
}

void ImageViewer::setHorizontalScrollPosition(int value)
{
    this->_view->horizontalScrollBar()->setValue(value);
}
