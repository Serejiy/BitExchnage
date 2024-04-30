#include <QDebug>
#include <QCryptographicHash>
#include <QMessageAuthenticationCode>
#include <QUrlQuery>
#include <QUrl>

#include <QJsonDocument>
#include <QJsonObject>

#include "binanceclient.h"

binanceClient::binanceClient(QByteArray apiKey, QByteArray secretKey, QObject* parent):
    _apiKey(apiKey)
    , _secretKey(secretKey)
{
    _networkManagerPrice = new QNetworkAccessManager(this);
    _networkManagerCandle = new QNetworkAccessManager(this);
    _networkManagerSTime = new QNetworkAccessManager(this);

    connect(_networkManagerPrice, &QNetworkAccessManager::finished,
            this, &binanceClient::replyFinishedPrice, Qt::DirectConnection);
    connect(_networkManagerCandle, &QNetworkAccessManager::finished,
            this, &binanceClient::replyFinishedCandle, Qt::DirectConnection);
    connect(_networkManagerSTime, &QNetworkAccessManager::finished,
            this, &binanceClient::replyFinishedSTime, Qt::DirectConnection);
}

QByteArray binanceClient::getHMAC(const QString & message)
{
    QUrl url(message);
    return QMessageAuthenticationCode::hash(url.toEncoded(), _secretKey, QCryptographicHash::Sha256).toHex();
}

void binanceClient::getServerTime()
{
    QUrl url(QString(API_URL) + QString(TIME_V1));
    QNetworkRequest netReq;
    netReq.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    netReq.setUrl(url);
    _networkManagerSTime->get(netReq);
}

void binanceClient::getAllPrices()
{
    QUrl url(QString(API_URL) + QString(PRICE_V1));
    QNetworkRequest netReq;
    netReq.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    netReq.setUrl(url);
    qDebug() << url;
    //_networkManager->get(netReq);
}

void binanceClient::getSymbolPrice(const QString &name)
{
    QUrl url(QString(API_URL) + QString(PRICE_V3) + QString("?symbol=") + name);
    QNetworkRequest netReq;
    netReq.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    netReq.setUrl(url);
    _networkManagerPrice->get(netReq);
}

void binanceClient::candleSticks(const QString &name, qulonglong startTime)
{
    QUrl url(QString(API_URL) + QString(CANDLESTICK) + QString("?symbol=") + name + QString("&interval=1m&startTime=") + QString::number(startTime));
    QNetworkRequest netReq;
    netReq.setSslConfiguration(QSslConfiguration::defaultConfiguration());
    netReq.setUrl(url);
    _networkManagerCandle->get(netReq);
}

void binanceClient::replyFinishedPrice(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    if(!data.isEmpty())
    {
        QJsonDocument jsondoc = QJsonDocument::fromJson(data);

        if(!jsondoc.isNull())
        {
            if(jsondoc.isObject())
            {
                QJsonObject obj = jsondoc.object();
                emit priceSignal(obj["price"].toString().toDouble());
            }
            else if(jsondoc.isArray())
            {
                foreach(QJsonValue element, jsondoc.array())
                {
                    QJsonObject node = element.toObject();
                    if(node["symbol"].toString() == "ADAETH")
                        qDebug() << node["symbol"].toString() << node["price"].toString().toDouble();
                }
            }
        }
    }

    reply->deleteLater();
}

void binanceClient::replyFinishedCandle(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    if(!data.isEmpty())
    {
        QJsonDocument jsondoc = QJsonDocument::fromJson(data);
        if(!jsondoc.isNull())
        {
            if(jsondoc.isArray())
            {
                emit candleSticksSignal(jsondoc.array());
            }
        }
    }

    reply->deleteLater();
}

void binanceClient::replyFinishedSTime(QNetworkReply *reply)
{
    if (reply->error() != QNetworkReply::NoError)
    {
        qDebug() << reply->errorString();
        reply->deleteLater();
        return;
    }

    QByteArray data = reply->readAll();
    QJsonDocument jsondoc = QJsonDocument::fromJson(data);
    if(!jsondoc.isNull())
    {
        if(jsondoc.isObject())
        {
            QJsonObject obj = jsondoc.object();
            emit serverTimeSignal(obj["serverTime"].toVariant().toULongLong());
        }
    }

    reply->deleteLater();
}
