#ifndef HOMEPAGE_H
#define HOMEPAGE_H

#include <QWidget>
#include <QStringListModel>

#include "include/environment.h"

QT_BEGIN_NAMESPACE
namespace Ui { class HomePage; }
QT_END_NAMESPACE

class HomePage : public QWidget
{
    Q_OBJECT
public:
    explicit HomePage(QWidget *parent = nullptr);
    virtual ~HomePage();

private:
    void openViewer(const QString &path);
    void addItemPreviousProjectList(const QString &path);
    void loadNewProjectFormContent();

private slots:
    void on_chapterViewerOpenButton_clicked();

    void on_projectListView_doubleClicked(const QModelIndex &index);

    void on_srcSelectButton_clicked();

    void on_destButton_clicked();

    void on_submitButton_clicked();

    void on_ocrComboBox_currentIndexChanged(int index);

    void on_transComboBox_currentIndexChanged(int index);

signals:

private:
    Ui::HomePage *ui;

    QStringListModel *previousProjectListModel = nullptr;
    QStringList previousProjectList;

    // Create Project Form
    QString sourceDirectory;
    QString destDirectory;

    static const size_t PREV_PROJECT_LIST_SIZE_LIMIT = 11;
};

#endif // HOMEPAGE_H
