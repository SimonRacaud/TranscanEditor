#ifndef ZOOMWIDGET_H
#define ZOOMWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QLabel>

/**
 * @brief The ZoomWidget class : Slider used to manage the zoom in the editor
 */
class ZoomWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ZoomWidget(float zoom, QWidget *parent = nullptr);

public slots:
    /**
     * @brief setZoom : set local zoom value
     * @param value
     */
    void setZoom(float value);

signals:
    /**
     * @brief sigZoomChanged : When the zoom value has been updated
     * @param value : new zoom
     */
    void sigZoomChanged(float value);

private slots:
    void sliderMoved(int value);

private:
    float _zoom{0}; // value [0.01 to 5] => 1% to 500%

    QSlider *_slider;
    QLabel *_level;

};

#endif // ZOOMWIDGET_H
