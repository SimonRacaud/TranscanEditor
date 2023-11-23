#include "APIClient.h"

#include "model/Page.h"
#include <QJsonObject>
#include <QJsonDocument>

const std::unordered_map<APIClient::Target, QString> APIClient::_target = {
    {APIClient::Target::OCR, "ocr"},
    {APIClient::Target::CLEAN, "clean"},
    {APIClient::Target::TRANSLATE, "translate"},
};

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

    this->pushRequest(APIClient::OCR, body, callback, errFunc);
}

void APIClient::sendToClean(OCRPage const& page, NetCallback &callback, NetErrCallback &errCallback)
{
    QJsonObject json = page.serialize();
    QJsonDocument body(json);
    QByteArray bodyData(body.toJson());

    this->pushRequest(APIClient::CLEAN, bodyData, callback, errCallback);
}

void APIClient::sendToTranslate(OCRPage const& page, QString const& transService,
                                NetCallback &callback, NetErrCallback &errCallback)
{
    QJsonObject rootJson;
    rootJson["translationService"] = transService;
    rootJson["page"] = page.serialize();

    QJsonDocument bodyJson(rootJson);
    QByteArray body(bodyJson.toJson());

    this->pushRequest(APIClient::TRANSLATE, body, callback, errCallback);
}

void APIClient::sendToRender(OCRPage const&, RenderConfig const&,
                             NetCallback &, NetErrCallback &)
{

    throw std::runtime_error("APIClient::sendToRender : the method is deprecated");

//    QJsonObject rootJson;
//    rootJson["config"] = config.serialize();
//    rootJson["page"] = page.serialize();

//    QJsonDocument bodyJson(rootJson);
//    QByteArray body(bodyJson.toJson());

//    this->pushRequest(APIClient::RENDER, body, callback, errCallback);
}

void APIClient::abortRequests()
{
    for (shared_ptr<CoreRequest> const &req : _pendingReplies) {
        req->reply->disconnect(req->reply, &QNetworkReply::finished, 0, 0);
        req->reply->deleteLater();
    }
    _pendingReplies.clear();
    _requestQueue.clear();
}

bool APIClient::pendingReply() const
{
    return !(_pendingReplies.isEmpty() && _requestQueue.isEmpty());
}

/** Protected **/

void APIClient::tryFlushRequest()
{
    const qsizetype pendingCount = this->_pendingReplies.size();

    if (pendingCount < MAX_PENDING && !_requestQueue.isEmpty()) {
        // Can send more request
        const qsizetype toSend = MAX_PENDING - pendingCount;
        for (qsizetype i = 0; i < toSend; i++) {
            if (this->_requestQueue.isEmpty())
                break; // No more to process
            shared_ptr<CoreRequest> const &req = this->_requestQueue.dequeue();

            qDebug() << "APIClient : Request sent for " << req->target
                        << " at " << QDateTime::currentMSecsSinceEpoch();
            this->sendRequest(req);
        }
    }
}

void APIClient::pushRequest(Target target, QByteArray const &body,
                 NetCallback &callback, NetErrCallback &errFunc)
{
    this->_requestQueue.push_back(std::make_shared<CoreRequest>(
                                      APIClient::_target.at(target),
                                      body,
                                      callback,
                                      errFunc));
    qDebug() << "APIClient : Request pushed for " << APIClient::_target.at(target)
             << " at " << QDateTime::currentMSecsSinceEpoch();
    this->tryFlushRequest();
}

void APIClient::sendRequest(shared_ptr<CoreRequest> const req)
{ 
    QUrl url(QString("%1/%2").arg(_url, req->target));
    QNetworkRequest request(url);
    request.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");
    request.setTransferTimeout(NET_TIMEOUT_MS);
    QNetworkReply *reply = _netManager.post(request, req->body);

    QObject::connect(reply, &QNetworkReply::finished, req.get(), &CoreRequest::onReceiveResponse);
    // QObject::connect(reply, &QNetworkReply::errorOccurred, [reply](QNetworkReply::NetworkError) {});
    QObject::connect(req.get(), &CoreRequest::onFinish, this, &APIClient::onReceiveReply);

    req->reply = reply;
    this->_pendingReplies.push_back(req);
}

void APIClient::onReceiveReply(QNetworkReply *reply, QByteArray const &rawData,
                               NetErrCallback errorFun, NetCallback successFun)
{
    qDebug() << "APIClient : Response received at "
               << QDateTime::currentMSecsSinceEpoch();

    this->_pendingReplies.removeIf([reply](shared_ptr<CoreRequest> const &req) {
       return req->reply == reply;
    });
    if (reply->error() != QNetworkReply::NoError) {
        qDebug() << "Network Error: " << reply->errorString()
                 << reply->error() << " Body: " << rawData;
        this->abortRequests();
        errorFun(reply->errorString());
        emit this->sigNetError(reply->errorString());
    } else {
        QJsonDocument doc = QJsonDocument::fromJson(rawData);
        QJsonObject body = doc.object();
        // Convert JSON to OCRPage
        OCRPage page;
        try {
            page = OCRPage::deserialize(body);
            successFun(page);
        } catch (std::exception const &err) {
            qDebug() << "Network Error: invalid body. " << err.what();
        }
    }
    reply->deleteLater();
    // Send more request if there are any
    this->tryFlushRequest();
}
