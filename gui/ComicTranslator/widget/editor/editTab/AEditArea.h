#ifndef AEDITAREA_H
#define AEDITAREA_H

#include <QWidget>
#include <QGraphicsScene>
#include "QPageViewer.h"
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QList>
#include <QMovie>
#include <QLabel>
#include <QGraphicsProxyWidget>
#include <memory.h>

#include "include/model/OCRPage.h"
#include "include/environment.h"
#include "window/ViewerWindow.h"

using std::vector;

class AEditArea : public QWidget
{
    Q_OBJECT
public:
    explicit AEditArea(QString const &sourceDirectory = nullptr,
                       vector<OCRPage> const &pages = {}, QWidget *parent = nullptr);
    virtual ~AEditArea();

    void setLoadingState(bool enable);
    void setSourceDirectory(QString const &path);
    void showSourceView(bool enable);

    virtual void setPages(vector<OCRPage> const &pages);

private:
    void clearImageList();

protected slots:
    void refreshImageListSlot();

protected:
    virtual void resizeEvent(QResizeEvent *event) override;

signals:

private:
    QString _sourceDirectory;
    QHBoxLayout *_rootLayout;
    bool _sourceViewEnabled{false};
    ViewerWindow *_viewer{nullptr};

    QMovie *_loadingImg;
    QLabel *_loadingWidget;
    bool _loading{false};

protected:
    vector<OCRPage> _pages;
    QPageViewer *_view;
    QGraphicsScene *_scene;
    QList<QPixmap> _pixmapList;

    int _timePreviousResize{0};
};

#endif // AEDITAREA_H
