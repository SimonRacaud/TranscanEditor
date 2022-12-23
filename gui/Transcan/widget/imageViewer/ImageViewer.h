#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <iostream>
#include <QGraphicsPixmapItem>

#include "include/environment.h"
#include "model/Page.h"
#include "PageGraphicsView.h"

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    // Image to display from the OCRPage
    enum ImageMode {
        SOURCE, // Show source image
        CLEAN, // Show cleaned image
        RENDER // Show rendered image
    };

    explicit ImageViewer(ImageMode mode = SOURCE, QWidget *parent = nullptr);

    /**
     * @brief setPages Overwrite current pages with the ones given as parameter
     * @param pages
     */
    virtual void setPages(std::vector<OCRPage> const &pages);

    /**
     * @brief updatePage : Update a page from the current list.
     *  Will reload the while scene and recreate the widgets.
     * @param page
     */
    virtual void updatePage(OCRPage const &page);

    /**
     * @brief loadPagesFromPath Read every supported images located in path directory
     *  and overwrite the current OCRPage list.
     * @param path Path of a local directory
     */
    void loadPagesFromPath(QString const& path);

    /**
     * @brief getPages Export current pages
     * @return vector<OCRPage>
     */
    virtual std::vector<OCRPage> getPages();

    /**
     * @brief getPage Export page at index
     * @param index
     * @return
     */
    virtual OCRPage getPage(size_t index);

    /**
     * @brief setLoadingState - Enable or Disable the loading state of the widget.
     *   If enabled, will lock the edition and show a loading cursor.
     * @param enable
     */
    virtual void setLoadingState(bool enable);

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

    /**
     * @brief clearView Clear pages widgets
     */
    void clearView();

private:
    ImageMode _mode;
    // Loading icon
    QMovie *_loadingImg;
    QLabel *_loadingWidget;
    bool _loading{false};

    float _scale{1.0}; // Scale of the images
    int _timePreviousResize{0};

    size_t _imageWidth{0};

protected:
    QGraphicsItemGroup *_pageGroup{nullptr}; // Group of page items
    std::vector<OCRPage> _pages;
    PageGraphicsView *_view;
    QGraphicsScene *_scene;
    QList<QPixmap> _pixmapList; // Original images buffer
};

#endif // IMAGEVIEWER_H
