#include "ViewerWindow.h"
#include "include/env_config.h"
#include "include/env_messages.h"

#include <QSizePolicy>
#include <QKeyEvent>

const QList<QString> ViewerWindow::SUPPORTED_EXTENSION = {
    "*.png",
    "*.jpg",
    "*.jpeg"
};

ViewerWindow::ViewerWindow(QWidget *parent, bool integratedMode): QWidget{parent}
{
    this->_integratedMode = integratedMode;
    QVBoxLayout *mainlayout = new QVBoxLayout(this);
    QHBoxLayout *headerLayout = nullptr;
    QWidget *headerWidget = nullptr;

    // HEADER
    if (!integratedMode) {
        headerLayout = new QHBoxLayout();
        headerWidget = new QWidget();
        headerWidget->setLayout(headerLayout);
        headerWidget->setObjectName("ViewerHeader");
        this->_backButton = new QPushButton(TXT_GO_BACK);
        this->_title = new QLabel(TXT_CHAPTER_VIEWER);
        this->_openFolderButton = new QPushButton(TXT_OPEN_FOLDER);
        headerLayout->addWidget(this->_backButton, 0, Qt::AlignLeft);
        headerLayout->addWidget(this->_title, 0, Qt::AlignCenter);
        this->_title->setAlignment(Qt::AlignRight);
        this->_title->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
        this->_title->setMaximumWidth(TITLE_MAX_SIZE);
        headerLayout->addWidget(this->_openFolderButton, 0, Qt::AlignRight);
        connect(this->_openFolderButton, &QPushButton::released, this, &ViewerWindow::openFolderSlot);
        connect(this->_backButton, &QPushButton::released, this, &ViewerWindow::goBackSlot);
    }
    // CONTENT
    QWidget *imageListContainer = new QWidget;
    this->_scrollArea = new QScrollArea();
    this->_scrollArea->setWidgetResizable(true);
    this->_scrollArea->setWidget(imageListContainer);
    if (integratedMode) { // Disable vertical scroll bar
        //this->_scrollArea->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
        //this->_scrollArea->verticalScrollBar()->resize(0, 0);
        this->_scrollArea->verticalScrollBar()->setFixedWidth(15);
        this->_scrollArea->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    }
    this->_scrollArea->setStyleSheet("border: none");
    this->_imageList = new QVBoxLayout(imageListContainer);
    this->_imageList->setAlignment(Qt::AlignHCenter);
    this->_imageList->setSpacing(0);
    this->_imageList->setContentsMargins(0, 0, 0, 0);
    //
    if (!integratedMode) {
        mainlayout->addWidget(headerWidget);
    }
    mainlayout->addWidget(this->_scrollArea);
    mainlayout->setSpacing(0);
    mainlayout->setContentsMargins(0, 0, 0, 0);
    if (!integratedMode) {
        this->resize(WIN_INIT_WIDTH, WIN_INIT_HEIGHT);
    }
}

/** Slots **/

void ViewerWindow::openFolderSlot()
{
    const QString folderPath = QFileDialog::getExistingDirectory(this, TXT_IMG_DIR);

    if (folderPath.isEmpty() == false) {
        this->loadDirectory(folderPath);
    }
}

void ViewerWindow::goBackSlot()
{
    this->close();
    if (this->parentWidget() != NULL) {
        this->parentWidget()->show();
    }
}

void ViewerWindow::setVerticalScrollPosition(int value)
{
    this->_scrollArea->verticalScrollBar()->setValue(value);
}

void ViewerWindow::setHorizontalScrollPosition(int value)
{
    this->_scrollArea->horizontalScrollBar()->setValue(value);
}

/** Public methods **/

QScrollBar *ViewerWindow::getVerticalScroll() const
{
    return this->_scrollArea->verticalScrollBar();
}

QScrollBar *ViewerWindow::getHorizontalScroll() const
{
    return this->_scrollArea->horizontalScrollBar();
}

void ViewerWindow::scale(float scale)
{
    this->_zoom *= scale;
    this->resizeImages(this->_scrollArea->width());
}

void ViewerWindow::loadDirectory(QString const &dirPath)
{
    QDir dir(dirPath);
    QStringList filter;
    QFileInfoList filelistinfo;

    // Remove previous images
    this->clearImageList();
    // Set viewer Title
    if (_title) {
        QString title = dirPath;
        this->_title->setText(title);
    }
    // Filter image files
    for (qsizetype i = 0; i < ViewerWindow::SUPPORTED_EXTENSION.size(); i++) {
        filter << ViewerWindow::SUPPORTED_EXTENSION.at(i);
    }
    dir.setNameFilters(filter);
    // Load Images
    filelistinfo = dir.entryInfoList();
    foreach (const QFileInfo &fileinfo, filelistinfo) {
        QString imageFile = fileinfo.absoluteFilePath();
        QImage *img = new QImage();
        QLabel *myLabel = new QLabel();

        qInfo("Load image: %s", imageFile.toStdString().c_str());
        img->load(imageFile);
        myLabel->setPixmap(QPixmap::fromImage(*img));
        this->_imageList->addWidget(myLabel);
        this->_listImage.append(img);
        this->_listImgLabel.append(myLabel);
    }
    this->resizeImages(this->_scrollArea->width());
}

/** PROTECTED **/

void ViewerWindow::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    // Resize Image List width
    int now = QTime::currentTime().msecsSinceStartOfDay();
    if (now - this->_timePreviousResize > TIME_REFRESH_RESIZE) {
        this->resizeImages(this->_scrollArea->width());
        this->_timePreviousResize = now;
    }
}

void ViewerWindow::keyPressEvent(QKeyEvent *event)
{
    QWidget::keyPressEvent(event);
    if (!_integratedMode) {
        if (event->key() == Qt::Key::Key_Plus || event->key() == Qt::Key::Key_ZoomIn) {
            this->scale(1.0 + ZOOM_SHIFT);
        } else if (event->key() == Qt::Key::Key_Minus || event->key() == Qt::Key::Key_ZoomOut) {
            this->scale(1.0 - ZOOM_SHIFT);
        }
    }
}

/** PRIVATE **/

void ViewerWindow::resizeImages(const int width)
{
    if (this->_listImgLabel.size() != this->_listImage.size()) {
        qWarning("Warning: ViewerWindow::resizeEvent list sizes don't match. That must never occur.");
        return;
    }
    int newWidth = (width - this->_scrollArea->verticalScrollBar()->width()) /* * (_zoom / 100.0f)*/;
    for (qsizetype i = 0; i < this->_listImgLabel.size(); i++) {
        QLabel *label = this->_listImgLabel.at(i);
        QImage *img = this->_listImage.at(i);
        QPixmap pixmap = QPixmap::fromImage(*img).scaledToWidth(newWidth);

        label->setPixmap(pixmap);
    }
}

void ViewerWindow::clearImageList()
{
    if (this->_imageList->layout() != NULL) {
        QLayoutItem* item;
        while ((item = this->_imageList->takeAt(0)) != NULL) {
            QWidget *widget = item->widget();
            if (widget)
                widget->deleteLater();
            delete item;
        }
        this->_listImage.clear();
        this->_listImgLabel.clear();
    }
}
