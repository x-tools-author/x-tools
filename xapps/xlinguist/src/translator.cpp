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
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonParseError>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QNetworkRequest>

#include "tsfilemanager.h"

std::atomic_bool Translator::s_requestInterrupted = false;
Translator::Translator(const QString &from,
                       const QString &to,
                       const QString &fileName,
                       const QString &sourceText,
                       int sourceLineNumber)
    : QRunnable()
    , m_from(from)
    , m_to(to)
    , m_fileName(fileName)
    , m_sourceText(sourceText)
    , m_sourceLineNumber(sourceLineNumber)
{}

Translator::~Translator() {}

void Translator::setRequestInterrupted(bool interrupted)
{
    s_requestInterrupted.store(interrupted);
}

void Translator::run()
{
    if (s_requestInterrupted.load()) {
        return;
    }

    QString from = "auto";
    QString to = m_to;
    QString text = m_sourceText;
    QString baseUrl = "https://translate.googleapis.com/translate_a/single?";
    QString params = QString("client=gtx&sl=%1&tl=%2&dt=t&q=%3").arg(from, to, text);
    QUrl apiUrl(baseUrl + params);
#if 0
    qInfo() << apiUrl.toString();
#endif

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

    QByteArray bytes = reply->readAll();
    QString tmp = QString::fromUtf8(bytes);
    reply->deleteLater();

    QJsonParseError parseError;
    QJsonDocument jsonDoc = QJsonDocument::fromJson(bytes, &parseError);
    if (jsonDoc.isNull() || !jsonDoc.isArray()) {
        qWarning() << "JSON parse error:" << parseError.errorString();
    }

    QJsonArray jsonArray = jsonDoc.array();
    if (jsonArray.isEmpty()) {
        qWarning() << "Empty translation result.";
        return;
    }

    QJsonArray a1 = jsonArray.at(0).toArray();
    if (a1.isEmpty()) {
        qWarning() << "Empty translation result array.";
        return;
    }

    QJsonArray a11 = a1.at(0).toArray();
    if (a11.isEmpty()) {
        qWarning() << "Empty translation result sub-array.";
        return;
    }

    QString a111 = a11.at(0).toString();
    gTsFileMgr.updateTranslation(m_fileName, a111, m_sourceLineNumber);
}
