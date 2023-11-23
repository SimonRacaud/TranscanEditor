#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QQueue>

#include "model/configModels.h"
#include "model/Page.h"
#include "include/env_config.h"
#include "CoreRequest.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

#define MAX_PENDING 2

/**
 * @brief The APIClient class : Network interface of the application
 * Used to communicate with the CORE module
 */
class APIClient : public QObject
{
    Q_OBJECT
public:
    enum Target {
        OCR,
        CLEAN,
        TRANSLATE
    };

    APIClient(QString const &url = CORE_API_URL);

    /**
     * @brief sendToOCR Text detection and extraction
     * @param config Configuration of the project
     * @param pageIdx Index of the page process
     * @param pageImgPath Path of the page to process
     */
    void sendToOCR(ProjectConfig const& config,
                   unsigned int pageIdx,
                   QString const &pageImgPath,
                   NetCallback &callback,
                   NetErrCallback &errFunc);

    /**
     * @brief sendToClean Remove text from image
     * @param page Page to process
     */
    void sendToClean(OCRPage const &page,
                     NetCallback &callback,
                     NetErrCallback &errCallback);

    /**
     * @brief sendToTranslate Translate image's sentences
     * @param page Page to process
     * @param transService Translation service
     */
    void sendToTranslate(OCRPage const &page, QString const& transService,
                         NetCallback &callback,
                         NetErrCallback &errCallback);

    /**
     * @brief sendToRender Render page
     * @param page Page to render
     * @param config Default configuration of the render service
     */
    void sendToRender(OCRPage const &page, RenderConfig const &config,
                      NetCallback &callback,
                      NetErrCallback &errCallback);

    /**
     * @brief abortRequests : stop waiting for any reply
     */
    void abortRequests();

    /**
     * @brief pendingReply
     * @return Return True if there is one or more pending reply
     */
    bool pendingReply() const;

signals:
    void sigNetError(QString const &message);

protected:
    /**
     * @brief tryFlushRequest : If Possible, send queued request(s)
     */
    void tryFlushRequest();

    /**
     * @brief pushRequest : Add request to the queue
     * @param target : Target endpoint
     * @param body : Request body
     * @param callback : Callback called in case of success
     * @param errFunc : Callback called in case of error
     */
    void pushRequest(Target target, QByteArray const &body,
                     NetCallback &callback, NetErrCallback &errFunc);

    /**
     * @brief sendRequest Send POST request to the core
     */
    void sendRequest(std::shared_ptr<CoreRequest> const req);
protected slots:
    /**
     * @brief onReceiveReply : Called when a reply has been received
     */
    void onReceiveReply(QNetworkReply *reply, QByteArray const &rawData,
                        NetErrCallback errorFun, NetCallback successFun);

private:
    QString _url;
    QNetworkAccessManager _netManager;
    QQueue<std::shared_ptr<CoreRequest>> _requestQueue;
    QList<std::shared_ptr<CoreRequest>> _pendingReplies;

    static const std::unordered_map<Target, QString> _target;
};

#endif // APICLIENT_H
