#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);
    virtual ~HomePage();

private slots:
    void on_chapterViewerOpenButton_clicked();

signals:

private:
    Ui::HomePage *ui;
};

#endif // HOMEPAGE_H
