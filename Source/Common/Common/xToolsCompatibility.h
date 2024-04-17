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

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define xToolsSkipEmptyParts Qt::SkipEmptyParts
#else
#define xToolsSkipEmptyParts QString::SkipEmptyParts
#endif

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

// QByteArray QByteArray::toHex(char separator = '\0') const
static QByteArray xToolsByteArrayToHex(const QByteArray &source, char separator = '\0')
{
    if (source.isEmpty()) {
        return source;
    }

    auto toHex = [](quint8 value) -> char { return "0123456789abcdef"[value & 0xF]; };

    const int length = separator ? (source.size() * 3 - 1) : (source.size() * 2);
    QByteArray hex(length, Qt::Uninitialized);
    char *hexData = hex.data();
    const uchar *data = reinterpret_cast<const uchar *>(source.constData());
    for (int i = 0, o = 0; i < source.size(); ++i) {
        hexData[o++] = toHex(data[i] >> 4);
        hexData[o++] = toHex(data[i] & 0xf);

        if ((separator) && (o < length))
            hexData[o++] = separator;
    }
    return hex;
}
