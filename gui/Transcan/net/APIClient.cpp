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
    QJsonObject configObj = obj["config"].toObject();
    configObj["srcImgPath"] = pageImgPath;
    obj["config"] = configObj;
    QJsonDocument doc(obj);
    QByteArray body(doc.toJson());

    this->sendRequest("ocr", body, callback, errFunc);
}

void APIClient::sendToClean(OCRPage const& page, NetCallback &callback, NetErrCallback &errCallback)
{
    QJsonObject json = page.serialize();
    QJsonDocument body(json);
    QByteArray bodyData(body.toJson());

    this->sendRequest("clean", bodyData, callback, errCallback);
}

void APIClient::sendToTranslate(OCRPage const& page, QString const& transService,
                                NetCallback &callback, NetErrCallback &errCallback)
{
    QJsonObject rootJson;
    rootJson["translationService"] = transService;
    rootJson["page"] = page.serialize();

    QJsonDocument bodyJson(rootJson);
    QByteArray body(bodyJson.toJson());

    this->sendRequest("translate", body, callback, errCallback);
}

void APIClient::sendToRender(OCRPage const& page, RenderConfig const& config,
                             NetCallback &callback, NetErrCallback &errCallback)
{
    QJsonObject rootJson;
    rootJson["config"] = config.serialize();
    rootJson["page"] = page.serialize();

    QJsonDocument bodyJson(rootJson);
    QByteArray body(bodyJson.toJson());

    this->sendRequest("render", body, callback, errCallback);
}

void APIClient::abortRequests()
{
    for (QNetworkReply *reply : _pendingReplies) {
        reply->disconnect(reply, &QNetworkReply::finished, 0, 0);
        reply->deleteLater();
    }
    _pendingReplies.clear();
}

bool APIClient::pendingReply() const
{
    return !_pendingReplies.empty();
}

/** Protected **/

void APIClient::sendRequest(QString const& target, QByteArray const &body,
                            NetCallback &callback, NetErrCallback &errFunc)
{
    QUrl url(QString("%1/%2")
             .arg(_url, target));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    QNetworkReply *reply = _netManager.post(request, body);

    QObject::connect(reply, &QNetworkReply::finished, [reply, callback, errFunc, this, target]() {
        this->_pendingReplies.removeOne(reply);
        if (reply->error() != QNetworkReply::NoError){
            qDebug() << "Network Error: " << reply->errorString() << reply->error();
            errFunc(reply->errorString());
        } else {
            QByteArray const &rawData = reply->readAll();
            qDebug() << "Network reply received for target : " << target << ".";
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