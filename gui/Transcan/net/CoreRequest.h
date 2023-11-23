#ifndef COREREQUEST_H
#define COREREQUEST_H

#include <QString>
#include <QNetworkReply>
#include "model/Page.h"

using NetCallback = std::function<void(OCRPage const& page)>;
using NetErrCallback = std::function<void(QString const& errMsg)>;

class CoreRequest : public QObject
{
    Q_OBJECT
public:
    CoreRequest(
            QString const &target,
            QByteArray const &body,
            NetCallback &success,
            NetErrCallback &error);
public:
    QString const target;
    QByteArray const body;
    NetCallback successFun;
    NetErrCallback errorFun;

    QNetworkReply *reply;

signals:
    void onFinish(QNetworkReply *, QByteArray const &, NetErrCallback&, NetCallback&);

public slots:
    void onReceiveResponse();
};

#endif // COREREQUEST_H
