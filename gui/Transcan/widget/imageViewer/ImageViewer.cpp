#include "ImageViewer.h"

#include <QHBoxLayout>
#include <QScrollBar>
#include <QMovie>
#include <QKeyEvent>
#include <QtGlobal>
#include <QDir>
#include <stdexcept>

using namespace std;

ImageViewer::ImageViewer(ImageMode mode, QWidget *parent) : QWidget{parent}, _mode(mode)
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

void ImageViewer::setPages(vector<OCRPage> const &pages)
{
    this->_pages = pages;
    size_t offsetY = 0;
    size_t maxImgWidth = 0;

    this->clearView(); // Remove current pages

    QList<QGraphicsItem *> pageItems;
    for (const OCRPage &page : pages) {
        QPixmap img;
        QString path;
        if (_mode == SOURCE) {
            path = page.imagePath;
        } else if (_mode == CLEAN) {
            path = page.cleanImagePath;
        } else {
            path = page.renderImagePath;
        }
        bool loaded = img.load(path);
        if (!loaded) { // Failed to load image
            if (_mode != SOURCE) {
                loaded = img.load(page.imagePath); // Load source image instead
            }
            if (_mode == SOURCE || !loaded) {
                throw std::invalid_argument("ImageViewer::setPages Failed to load image "+path.toStdString());
            }
        }
        this->_pixmapList.append(img);
        QGraphicsDropShadowEffect* effect = new QGraphicsDropShadowEffect();
        effect->setBlurRadius(EDITOR_PAGE_SHADOW_RADIUS);
        effect->setXOffset(0);
        effect->setColor(EDITOR_PAGE_SHADOW_COLOR);
        auto *item = new QGraphicsPixmapItem(img);
        item->setGraphicsEffect(effect);
        item->setPos(0, offsetY);
        pageItems.push_back(item->topLevelItem());
        offsetY += img.height();
        maxImgWidth = qMax(maxImgWidth, (size_t)img.width());
    }
    if (_imageWidth == 0 && offsetY > 0/* The images were found */) {
        // First images loaded. Not an update of a previous display.
        _imageWidth = maxImgWidth;
        this->_scene->setSceneRect(QRectF(0, 0, _imageWidth, offsetY));
    }
    this->_pageGroup = this->_scene->createItemGroup(pageItems);
}

void ImageViewer::updatePage(OCRPage const &page)
{
    unsigned int index = page.index;

    // Check page index
    if (_pages.size() <= index || _pixmapList.size() <= index) {
        throw std::invalid_argument("ImageViewer::updatePage Invalid page index.");
    }
    // Check new image access
    QString filePath;
    if (_mode == SOURCE) {
        filePath = page.imagePath;
    } else if (_mode == CLEAN) {
        filePath = page.cleanImagePath;
    } else if (_mode == RENDER) {
        filePath = page.renderImagePath;
    }
    QImageReader newImgMeta(filePath);
    bool success = newImgMeta.canRead();
    if (!success) {
        qDebug() << "ImageViewer::updatePage Fail to load " << filePath;
        throw std::runtime_error("ImageViewer::updatePage, failed to load new image.");
    }
    //
    _pages[index] = page; // Update page data
    this->setPages(_pages); // Load pages again
}

void ImageViewer::loadPagesFromPath(QString const &path)
{
    vector<OCRPage> pages;
    this->clearView();

    try {
        QDir dir(path);
        QStringList filter;
        QFileInfoList filelistinfo;

        // Filter image files
        for (size_t i = 0; i < (size_t)SUPPORTED_EXTENSION.size(); i++) {
            filter << SUPPORTED_EXTENSION.at(i);
        }
        dir.setNameFilters(filter);
        // Load images
        filelistinfo = dir.entryInfoList();
        foreach (const QFileInfo &fileinfo, filelistinfo) {
            QString imageFile = fileinfo.absoluteFilePath();
            OCRPage page;
            page.imagePath = imageFile;
            pages.push_back(page);
        }
        if (pages.size()) {
            this->setPages(pages);
        }
    } catch (std::exception const &err) {
        std::cerr << "Error: " << err.what() << std::endl;
        throw std::invalid_argument("Unable to access source directory.");
    }
    if (pages.size() == 0) {
        throw std::invalid_argument("The source directory doesn't contain any valid image.");
    }
}

std::vector<OCRPage> ImageViewer::getPages()
{
    std::vector<OCRPage> pages;

    for (size_t i = 0; i < _pages.size(); i++) {
        // calling getPage() to use EditTab's override instance.
        pages.push_back(this->getPage(i));
    }
    return pages;
}

OCRPage ImageViewer::getPage(size_t index)
{
    if (index >= _pages.size()) {
        throw std::invalid_argument("ImageViewer::getPage Invalid page index.");
    }
    return _pages[index];
}

void ImageViewer::setLoadingState(bool enable)
{
    this->_loading = enable;
    if (enable) {
        this->_loadingWidget->show();
        _loadingWidget->move(this->width() / 2 - _loadingWidget->width() / 2,
                             this->height() / 2 - _loadingWidget->height() / 2);
        _loadingImg->start();
    } else {
        this->_loadingImg->stop();
        this->_loadingWidget->hide();
    }
}

void ImageViewer::scale(float scale)
{
    this->_view->scale(scale, scale);
}

void ImageViewer::setZoom(float percent)
{
    int viewWidth = this->_view->width();

    if (percent < 0.01 || percent > 5.0) {
        return; // Abort
    }
    if (this->_view->verticalScrollBar()->isEnabled()) {
        viewWidth -= this->_view->verticalScrollBar()->width();
    }
    if (!viewWidth)
        return;
    float targetWidth = (float)viewWidth * percent;
    float currentWidth = (float)viewWidth * _zoom;
    if (!currentWidth)
        return;
    float newScale = targetWidth / currentWidth;
    if (!newScale)
        return;
    this->scale(newScale);
    this->_zoom = percent;
}

float ImageViewer::getZoom() const
{
    return _zoom;
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

void ImageViewer::clearView()
{
    this->_pageGroup = nullptr;
    this->_pixmapList.clear();
    this->_scene->clear();
}

void ImageViewer::emitScrollPosition()
{
    emit this->horizontalScrollValueChanged(this->_view->horizontalScrollBar()->value());
    emit this->verticalScrollValueChanged(this->_view->verticalScrollBar()->value());
}

/** SLOTS **/

void ImageViewer::refreshImageListSlot()
{
    int viewWidth = this->_view->width() /* Widget borders */ - 4;

    if (this->_view->verticalScrollBar()->isEnabled()) {
        viewWidth -= this->_view->verticalScrollBar()->width();
    }
    if (!_imageWidth || !viewWidth)
        return; // avoid div by zero - invalid data
    float scale = (float)viewWidth / (float)_imageWidth;
    // Scale widget for the image to fix the view width:
    this->scale(scale);
    // Update displayed images width:
    this->_imageWidth *= scale;
}

void ImageViewer::setVerticalScrollPosition(int value)
{
    this->_view->verticalScrollBar()->setValue(value);
}

void ImageViewer::setHorizontalScrollPosition(int value)
{
    this->_view->horizontalScrollBar()->setValue(value);
}
