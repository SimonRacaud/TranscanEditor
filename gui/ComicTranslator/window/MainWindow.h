#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "../page/home/HomePage.h"
#include "../page/editor/EditorPage.h"
#include "./ViewerWindow.h"

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
    EditorPage *_editor;
    ViewerWindow *_viewer;
};
#endif // MAINWINDOW_H
