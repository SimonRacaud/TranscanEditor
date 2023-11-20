#ifndef IMAGEVIEWER_H
#define IMAGEVIEWER_H

#include <QWidget>
#include <QLabel>
#include <iostream>
#include <QGraphicsPixmapItem>
#include <QGraphicsDropShadowEffect>

#include "include/environment.h"
#include "model/Page.h"
#include "PageGraphicsView.h"

class ImageViewer : public QWidget
{
    Q_OBJECT
public:
    // Image to display from the OCRPage
    enum ImageMode {
        NONE,   // Show images directly from filesystem
        SOURCE, // Show source image (local project)
        CLEAN, // Show cleaned image (local project)
        RENDER // Show rendered image (local project)
    };

    explicit ImageViewer(ImageMode mode = NONE, QWidget *parent = nullptr);

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
     * @return vector<OCRPage>
     */
    vector<OCRPage> loadPagesFromPath(QString const& path);

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

    /**
     * @brief ImageViewer::setZoom
     *  Set view zoom to a specific percentage
     * @param percent [0.01-5] -> 1% to 500%
     */
    void setZoom(float percent);

    /**
     * @brief ImageViewer::getZoom : Return the current zoom level
     * @return [0.01-5] -> 1% to 500%
     */
    float getZoom() const;

    /**
     * @brief emitScrollPosition : Force signals verticalScrollValueChanged
     *  & horizontalScrollValueChanged emittion
     */
    void emitScrollPosition();

    void foreachPage(std::function<void(OCRPage &)> fun);

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

    QGraphicsItemGroup *getPageGroup();

    /**
     * @brief clearView Clear pages widgets
     */
    void clearView();

    /**
     * @brief _getPathFromMode : Return the page's image location depending on the viewer's mode (Source, Clean or render)
     * @param page
     * @return target image path
     */
    QString _getPathFromMode(OCRPage const &page) const;

private:
    ImageMode _mode;
    // Loading icon
    QMovie *_loadingImg;
    QLabel *_loadingWidget;
    bool _loading{false};

    float _zoom{1.0}; // Scale of the images
    int _timePreviousResize{0};

    size_t _imageWidth{0};
    QGraphicsItemGroup *_pageGroup{nullptr}; // Group of page items

protected:
    std::vector<OCRPage> _pages;
    PageGraphicsView *_view;
    QGraphicsScene *_scene;
};

#endif // IMAGEVIEWER_H
