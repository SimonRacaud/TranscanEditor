#include "CoreRequest.h"
#include <QJsonDocument>

CoreRequest::CoreRequest(
      QString const &target,
      QByteArray const &body,
      NetCallback &success,
      NetErrCallback &error)
    : target(target), body(body), successFun(success), errorFun(error)
{}

void CoreRequest::onReceiveResponse()
{
    QByteArray const &rawData = reply->readAll();

    reply->disconnect(reply, &QNetworkReply::finished, nullptr, nullptr);

    qDebug() << "Network reply received for target : " << target << ".";
    emit this->onFinish(this->reply, rawData, errorFun, successFun);
}
