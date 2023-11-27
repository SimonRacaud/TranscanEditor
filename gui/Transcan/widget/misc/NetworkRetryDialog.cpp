#include "NetworkRetryDialog.h"
#include "window/MainWindow.h"

#include <QDebug>
#include <QCloseEvent>

#include <QMessageBox>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QTimer>

extern MainWindow *mainWindow;

NetworkRetryDialog::NetworkRetryDialog(QWidget *parent, APIClient &api) : QDialog(parent)
{
    this->setWindowTitle(tr("No network connection to API"));
    this->setModal(true);

    /// Setup view:
    QVBoxLayout *rootLay = new QVBoxLayout;
    rootLay->setContentsMargins(20, 20, 20, 20);
    rootLay->setAlignment(Qt::AlignTop);
    rootLay->setSpacing(10);
    this->setLayout(rootLay);

    QLabel *message = new QLabel(tr("Check your network connection and retry"));
    rootLay->addWidget(message, 0, Qt::AlignHCenter);
    rootLay->addSpacing(20);

    QHBoxLayout *buttonLay = new QHBoxLayout;
    buttonLay->setAlignment(Qt::AlignmentFlag::AlignHCenter);
    buttonLay->setSpacing(20);
    this->_abortButton = new QPushButton(tr("Abort"));
    this->_retryButton = new QPushButton(tr("Retry"));
    buttonLay->addWidget(_abortButton);
    buttonLay->addWidget(_retryButton);
    rootLay->addLayout(buttonLay);

    this->_loadingAnim = new LoadingAnimation(this);
    rootLay->addWidget(_loadingAnim, 0, Qt::AlignHCenter);

    connect(_abortButton, &QPushButton::clicked, this, &NetworkRetryDialog::_onAbort);
    connect(_retryButton, &QPushButton::clicked, this, &NetworkRetryDialog::_onretry);
    connect(this, &NetworkRetryDialog::abort, []() {
        if (mainWindow)
            mainWindow->setPage(Page::HOME);
    });
    connect(this, &NetworkRetryDialog::retry, &api, &APIClient::sendPing);
    connect(&api, &APIClient::pingResult, this, &NetworkRetryDialog::onPingResult);

    // Show dialogue
    this->exec();
}

/** SLOTS **/

void NetworkRetryDialog::onPingResult(bool isSuccess)
{
    if (isSuccess) {
        // Close dialogue
        this->_loadingAnim->stop();
        this->close();
    } else {
        // Unlock retry button
        QTimer::singleShot(500, [this]() {
            // Add delay for the loading animation to never be invisible to the user
            this->_retryButton->setDisabled(false);
            this->_loadingAnim->stop();
        });
    }
}

//

void NetworkRetryDialog::_onAbort()
{
    this->close(); // will call closeEvent method
}
void NetworkRetryDialog::_onretry()
{
    this->_retryButton->setDisabled(true);
    emit this->retry();
    this->_loadingAnim->start(this->size());
}

/** PRIVATE **/

void NetworkRetryDialog::closeEvent(QCloseEvent *e)
{
    if (QMessageBox::question(this,
                              tr("Abort ?"),
                              tr("You will go back to the home page and may lose your work, confirm ?")
                              ) == QMessageBox::StandardButton::Yes) {
        this->_loadingAnim->stop();
        emit this->abort();
        this->deleteLater();
    } else {
        e->ignore();
    }
}
