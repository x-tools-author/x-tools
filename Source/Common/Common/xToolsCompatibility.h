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
#define X_TOOLS_ABSTRACT_SOCKET_ERROR_OCCURRED &QAbstractSocket::errorOccurred
#else
#define X_TOOLS_ABSTRACT_SOCKET_ERROR_OCCURRED \
    static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#define X_TOOLS_WEB_SOCKET_ERROR_OCCURRED &QWebSocket::errorOccurred
#else
#define X_TOOLS_WEB_SOCKET_ERROR_OCCURRED \
    static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error)
#endif

static qint64 xToolsBirthTimeOfFile(const QFileInfo &info)
{
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    return info.birthTime().toMSecsSinceEpoch();
#else
    return info.created().toMSecsSinceEpoch();
#endif
}
