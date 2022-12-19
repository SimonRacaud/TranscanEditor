#include "APIClient.h"

#include "model/Page.h"
#include <QJsonObject>
#include <QJsonDocument>

APIClient::APIClient(QString const &url) : _url(url), _netManager(nullptr)
{
}

void APIClient::sendToOCR(ProjectConfig const& config,
                          unsigned int pageIdx,
                          QString const &pageImgPath,
                          NetCallback &callback,
                          NetErrCallback &errFunc)
{
    // Create JSON body object:
    QJsonObject obj;
    obj["index"] = QJsonValue((int)pageIdx);
    obj["config"] = config.serialize();
    obj["config"].toObject()["srcImgPath"] = pageImgPath;
    QJsonDocument doc(obj);
    QByteArray body(doc.toJson());

    this->sendRequest("ocr", body, callback, errFunc);
}

void APIClient::sendToClean(OCRPage const& page)
{
    // TODO
}

void APIClient::sendToTranslate(OCRPage const& page, QString const& transService)
{
    // TODO
}

void APIClient::sendToRender(OCRPage const& page, RenderConfig const& config)
{
    // TODO
}

/** Protected **/

void APIClient::sendRequest(QString const& target, QByteArray const &body,
                            NetCallback &callback, NetErrCallback &errFunc)
{
    QUrl url(QString("%1/%2/")
             .arg(_url)
             .arg(target));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = _netManager.post(request, body);

    QObject::connect(reply, &QNetworkReply::finished, [reply, callback, errFunc]() {
        if (reply->error() != QNetworkReply::NoError){
            qDebug() << "Network Error: " << reply->errorString() << reply->error();
            errFunc(reply->errorString());
        } else {
            qDebug() << "Network reply received."; // TODO
            QByteArray const &rawData = reply->readAll();
            QJsonDocument doc = QJsonDocument::fromJson(rawData);
            QJsonObject body = doc.object();
            // Convert JSON to OCRPage
            OCRPage page;
            try {
                page = OCRPage::deserialize(body);
            } catch (std::exception const &err) {
                qDebug() << "Network Error: invalid body. " << err.what();
                reply->disconnect(reply, &QNetworkReply::finished, 0, 0);
                reply->deleteLater();
                return;
            }
            //
            callback(page);
        }
        reply->disconnect(reply, &QNetworkReply::finished, 0, 0);
        reply->deleteLater();
    });
    this->_pendingReplies.push_back(reply);
}
