#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <QWidget>
#include "EditorView.h"
#include "net/APIClient.h"
#include "widget/misc/NetworkRetryDialog.h"

/**
 * @brief The EditorController class : Editor page's logic
 */
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

public slots:
    /**
     * @brief setZoom : Set Editor pages viewer zoom
     * @param value
     */
    void setZoom(float value);

    /**
     * @brief networkError : called when a network error occur with the CORE.
     * @param message
     */
    void networkError(QString const &message);

private:
    void setupEvents();

protected:
    virtual void keyPressEvent(QKeyEvent *event) override;

private:
    EditorTab _maxTab; // Maximum step accessed (to prevent skipping step).
    shared_ptr<ProjectConfig> _config;
    APIClient _api;
    NetworkRetryDialog *_networkDialog{nullptr};
};

#endif // EDITORCONTROLLER_H
