#include "AEditArea.h"
#include <QHBoxLayout>

AEditArea::AEditArea(
        QString const &sourceDirectory, vector<OCRPage> const &pages, QWidget *parent)
    : QWidget{parent}, _sourceDirectory(sourceDirectory)
{
    // Set background color
    QPalette pal = QPalette(QColor(EDITOR_EDITA_BG));
    this->setAutoFillBackground(true);
    this->setPalette(pal);
    //
    this->_rootLayout = new QHBoxLayout;
    // Editor
    this->_view = new QPageViewer();
    this->_scene = new QGraphicsScene();
    this->_view->setScene(_scene);
    this->_view->setRenderHint(QPainter::Antialiasing); // To test
    connect(_view, &QPageViewer::resizedSignal, this, &AEditArea::refreshImageListSlot);

    this->_viewer = new ViewerWindow(nullptr, true);
    this->_viewer->setVisible(false);
    //
    this->_rootLayout->addWidget(_viewer, 100);
    this->_rootLayout->addWidget(_view, 100);
    this->_rootLayout->setSpacing(0);
    this->_rootLayout->setContentsMargins(0, 0, 0, 0);
    this->setLayout(_rootLayout);
    // Synchronize scroll Editor and Viewer
    connect(this->_view->verticalScrollBar(), &QScrollBar::valueChanged, this->_viewer, &ViewerWindow::setVerticalScrollPosition);
    connect(this->_viewer->getVerticalScroll(), &QScrollBar::valueChanged, this->_view->verticalScrollBar(), &QScrollBar::setValue);
    //
    if (!pages.empty()) {
        this->setPages(pages);
    }
    // Loading page
    this->_loadingWidget = new QLabel(this);
    this->_loadingImg = new QMovie();
    _loadingImg->setFileName("/media/work/personnal-projects/scanTranslator/asset/img/loading.gif"); // TODO : init properly
    this->_loadingWidget->setMovie(_loadingImg);
    size_t size = 80;
    _loadingImg->setScaledSize(QSize(size, size));
    _loadingWidget->resize(size, size);

    // DEBUG (TODO):
    //this->setLoadingState(true);
    vector<OCRPage> debug = { // TODO: debug
        { .imagePath = "/media/work/personnal-projects/scanTranslator/data/dataset/debug/10-o.jpg" },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/data/dataset/debug/14-o.jpg" },
        { .imagePath = "/media/work/personnal-projects/scanTranslator/data/dataset/debug/9-o.jpg" },
    };
    this->setSourceDirectory("/media/work/personnal-projects/scanTranslator/data/dataset/debug");
    //this->showSourceView(true);
    //this->setPages(debug); // ? Debug

}

AEditArea::~AEditArea()
{
    this->clearImageList();
}

/** Public methods **/

void AEditArea::setPages(vector<OCRPage> const &pages)
{
    this->_pages = pages;
    const int width = this->_view->width() - this->_view->verticalScrollBar()->width() - 3;
    size_t offsetY = 0;

    this->clearImageList();
    for (const OCRPage &page : pages) {
        QPixmap img(page.imagePath);
        this->_pixmapList.append(img);
        QPixmap imgResize = img.scaledToWidth(width);
        auto item = new QGraphicsPixmapItem(imgResize);
        item->setPos(0, offsetY);
        this->_scene->addItem(item);
        offsetY += imgResize.height();
    }
}

void AEditArea::showSourceView(bool enable)
{
    if (enable) {
        this->_viewer->setVisible(true);
    } else {
       this->_viewer->setVisible(false);
    }
}

void AEditArea::setSourceDirectory(QString const &path)
{
    this->_sourceDirectory = path;
    if (!path.isEmpty()) {
        this->_viewer->loadDirectory(path);
    }
}

void AEditArea::setLoadingState(bool enable)
{
    this->_loading = enable;
    if (enable) {
        _loadingWidget->move(this->width() / 2, this->height() / 2);
        _loadingImg->start();
    } else {
        this->_loadingImg->stop();
    }
}

/** Internal methods **/

void AEditArea::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Resize Image List width
    int now = QTime::currentTime().msecsSinceStartOfDay();
    if ((now - _timePreviousResize) > EDITOR_TIME_REFRESH_RESIZE) {
        if (_loading) {
            _loadingWidget->move(this->width() / 2, this->height() / 2);
        }
        this->_timePreviousResize = now;
    }
}

void AEditArea::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_Delete) {
        // TODO : remove currently selected EditAreaRect
    }
}


void AEditArea::clearImageList()
{
    // TODO: redraw edit area rect after clearing the scene
    //_scene->clear();
}

/** SLOTS **/

void AEditArea::refreshImageListSlot()
{
    int width = this->_view->width() - 3;
    size_t offsetY = 0;

    if (this->_view->verticalScrollBar()->isVisible()) {
        width -= this->_view->verticalScrollBar()->width();
    }
    this->clearImageList();
    for (const QPixmap &img : _pixmapList) {
        QPixmap imgResize = img.scaledToWidth(width);
        auto item = new QGraphicsPixmapItem(imgResize);
        item->setPos(0, offsetY);
        this->_scene->addItem(item);
        offsetY += imgResize.height();
    }
}
