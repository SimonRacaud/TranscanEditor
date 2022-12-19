#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStackedWidget>

#include "../page/home/HomePage.h"
#include "../page/editor/EditorController.h"
#include "./ViewerWindow.h"

enum class Page {
    HOME,
    EDITOR
};

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void showViewer(QString const &dirPath);

    void setPage(Page page);

    void setConfig(ProjectConfig const &config);

private:
    QStackedWidget *_stack;
    HomePage *_homePage;
    EditorController *_editor{nullptr};
    ViewerWindow *_viewer;
};
#endif // MAINWINDOW_H
