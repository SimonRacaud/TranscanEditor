#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "../page/home/homepage.h"
#include "./viewerwindow.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showViewer(QString const &dirPath);

private:
    QStackedWidget *_stack;
    HomePage *_homePage;
    ViewerWindow *_viewer;
};
#endif // MAINWINDOW_H
