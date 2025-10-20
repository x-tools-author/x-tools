/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "translator.h"

#include <QDebug>
#include <QEventLoop>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

Translator::Translator()
    : QRunnable()
{}

Translator::~Translator() {}

void Translator::run()
{
    QString from = "auto";
    QString to = "en";
    QString text = "Hello, world!";
    QString baseUrl = "https://translate.googleapis.com/translate_a/single?";
    QString params = QString("client=gtx&sl=%1&tl=%2&dt=t&q=%3").arg(from, to, text);
    QUrl apiUrl(baseUrl + params);
    QNetworkAccessManager manager;
    QNetworkRequest request(apiUrl);
    QNetworkReply *reply = manager.get(request);
    while (!reply->isFinished()) {
        QEventLoop loop;
        QObject::connect(reply, &QNetworkReply::finished, &loop, &QEventLoop::quit);
        loop.exec();
    }

    if (reply->error() != QNetworkReply::NoError) {
        // Handle error
        QString errorString = reply->errorString();
        reply->deleteLater();
        qInfo() << QString("Error: %1").arg(errorString);
    }
}