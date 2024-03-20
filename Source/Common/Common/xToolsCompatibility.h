/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QDateTime>
#include <QFileInfo>
#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define SAK_SIG_SOCKETERROROCCURRED &QAbstractSocket::errorOccurred
#else
#define SAK_SIG_SOCKETERROROCCURRED \
    static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#define SAK_SIG_WEBSOCKETERROROCCURRED &QWebSocket::errorOccurred
#else
#define SAK_SIG_WEBSOCKETERROROCCURRED \
    static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error)
#endif

static qint64 sakBirthTimeOfFile(const QFileInfo &info)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    return info.birthTime().toMSecsSinceEpoch();
#else
    return info.created().toMSecsSinceEpoch();
#endif
}
