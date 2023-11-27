#ifndef NETWORKRETRYDIALOG_H
#define NETWORKRETRYDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QLabel>

#include "LoadingAnimation.h"
#include "net/APIClient.h"

class NetworkRetryDialog : public QDialog
{
    Q_OBJECT
public:
    NetworkRetryDialog(QWidget *parent, APIClient &api);

signals:
    /**
     * @brief abort : Go back to home page
     */
    void abort();
    /**
     * @brief retry : Send new API ping
     */
    void retry();

public slots:
    /**
     * @brief onPingResult : Called to notify the result of a network API's ping result
     * @param isSuccess : If the ping was a success
     */
    void onPingResult(bool isSuccess);

private slots:
    void _onAbort();
    void _onretry();

protected:
    /**
     * @brief closeEvent : called when the user press the cross button of the dialogue window
     * @param e
     */
    virtual void closeEvent(QCloseEvent *e) override;

private:
    QPushButton *_abortButton;
    QPushButton *_retryButton;
    LoadingAnimation *_loadingAnim;
};

#endif // NETWORKRETRYDIALOG_H
