#ifndef APICLIENT_H
#define APICLIENT_H

#include "include/models.h"
#include "include/model/OCRPage.h"
#include "include/environment.h"

#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>

using NetCallback = std::function<void(OCRPage const& page)>;
using NetErrCallback = std::function<void(QString const& errMsg)>;

class APIClient
{
public:
    APIClient(QString const &url = API_URL);

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
    void sendToClean(OCRPage const &page);

    /**
     * @brief sendToTranslate Translate image's sentences
     * @param page Page to process
     * @param transService Translation service
     */
    void sendToTranslate(OCRPage const &page, QString const& transService);

    /**
     * @brief sendToRender Render page
     * @param page Page to render
     * @param config Default configuration of the render service
     */
    void sendToRender(OCRPage const &page, RenderConfig const &config);

protected:
    /**
     * @brief sendRequest Send POST request to the core
     * @param target Target endpoint (ex: "ocr")
     * @param body JSON
     */
    void sendRequest(QString const& target, QByteArray const &body,
                     NetCallback &callback, NetErrCallback &errFunc);

private:
    QString _url;
    QNetworkAccessManager _netManager;
    QList<QNetworkReply*> _pendingReplies;
};

#endif // APICLIENT_H
