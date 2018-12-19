#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include "GetPublicIPThread.h"

#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QEventLoop>
#include <QDebug>

GetPublicIpThread::GetPublicIpThread(QObject *parent):QThread(parent)
{
    moveToThread(this);
}

GetPublicIpThread::~GetPublicIpThread()
{

}

void GetPublicIpThread::run()
{
    connect(this, SIGNAL(need2getPublicIp()), this, SLOT(getPublicIPActually()));

    exec();
}

void GetPublicIpThread::getPublicIPActually()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(pageUrl)));
    eventLoop.exec();

    QByteArray responseData;
    responseData = reply->readAll();

    QString ipString = QString(responseData);
    ipString = ipString.split('[').last();
    ipString = ipString.split(']').first();
    ipString = ipString.trimmed();

    if (ipString.isEmpty()){
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(backupPageUrl)));
        eventLoop.exec();
        responseData = reply->readAll();
        ipString = QString(responseData);
        ipString = ipString.trimmed();
    }

    emit publicIPChanged(ipString);
    emit publicIPChanged(QHostAddress(ipString));

    delete manager;
}

QString GetPublicIpThread::getPublicIPDirectly()
{
    QNetworkAccessManager *manager = new QNetworkAccessManager();

    QEventLoop eventLoop;
    QObject::connect(manager, SIGNAL(finished(QNetworkReply *)), &eventLoop, SLOT(quit()));
    QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(pageUrl)));
    eventLoop.exec();

    QByteArray responseData;
    responseData = reply->readAll();

    QString ipString = QString(responseData);
    ipString = ipString.split('[').last();
    ipString = ipString.split(']').first();
    ipString = ipString.trimmed();

    if (ipString.isEmpty()){
        QNetworkReply *reply = manager->get(QNetworkRequest(QUrl(backupPageUrl)));
        eventLoop.exec();
        responseData = reply->readAll();
        ipString = QString(responseData);
        ipString = ipString.trimmed();
    }

    delete manager;
    return ipString;
}

