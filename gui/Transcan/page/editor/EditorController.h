#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <QWidget>
#include "EditorView.h"
#include "net/APIClient.h"

class EditorController : public EditorView
{
    Q_OBJECT
public:
    EditorController(QWidget *parent = nullptr);
    ~EditorController();

    /**
     * @brief onStart Called when starting a new project edition (switching from home to editor page)
     * @param config
     */
    void onStart(ProjectConfig const &config);

    /**
     * @brief setTab Change Editor tab
     * @param tab
     */
    void setTab(EditorTab tab);

    void showSourcePageTab(bool enable);

    /**
     * @brief netError : Called when a network error fail.
     */
    void netError(QString const &message);

private:
    void setupEvents();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private slots:
    void showSourceButtonClickedSlot(bool checked);
    void extractButtonClickedSlot(bool checked);
    void cleanButtonClickedSlot(bool checked);
    void editButtonClickedSlot(bool checked);
    void saveButtonClickedSlot(bool checked);
    void exitButtonClickedSlot(bool checked);

private:
    ProjectConfig *_config = nullptr;
    APIClient _api;
};

#endif // EDITORCONTROLLER_H
