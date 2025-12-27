/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#define xWebSocketErrorOccurred &QWebSocket::errorOccurred
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xWebSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QWebSocket::error)
#else
#define xWebSocketErrorOccurred \
    static_cast<void (QWebSocket::*)(QAbstractSocket::SocketError)>(&QWebSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xLocalSocketErrorOccurred &QLocalSocket::errorOccurred
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xLocalSocketErrorOccurred qOverload<QLocalSocket::LocalSocketError>(&QLocalSocket::error)
#else
#define xLocalSocketErrorOccurred \
    static_cast<void (QLocalSocket::*)(QLocalSocket::LocalSocketError)>(&QLocalSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xTcpSocketErrorOccurred &QTcpSocket::errorOccurred
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xTcpSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QTcpSocket::error)
#else
#define xTcpSocketErrorOccurred \
    static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QTcpSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xUdpSocketErrorOccurred &QUdpSocket::errorOccurred
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xUdpSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QUdpSocket::error)
#else
#define xUdpSocketErrorOccurred \
    static_cast<void (QAbstractSocket::*)(QAbstractSocket::SocketError)>(&QUdpSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#define xComboBoxActivated &QComboBox::activated
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xComboBoxActivated qOverload<int>(&QComboBox::activated)
#else
#define xComboBoxActivated static_cast<void (QComboBox::*)(int)>(&QComboBox::activated)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#define xComboBoxIndexChanged &QComboBox::currentIndexChanged
#elif QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xComboBoxIndexChanged qOverload<int>(&QComboBox::currentIndexChanged)
#else
#define xComboBoxIndexChanged static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
#define xCheckStateChanged &QCheckBox::checkStateChanged
#else
#define xCheckStateChanged qOverload<int>(&QCheckBox::stateChanged)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xSkipEmptyParts Qt::SkipEmptyParts
#else
#define xSkipEmptyParts QString::SkipEmptyParts
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 8, 0)
#define xSerialPortErrorOccurred &QSerialPort::errorOccurred
#else
#define xSerialPortErrorOccurred \
    static_cast<void (QSerialPort::*)(QSerialPort::SerialPortError)>(&QSerialPort::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 9, 0)
#define xBytes2Hex(ba, c) ba.toHex(c)
#else
#define xBytes2Hex(ba, c) \
    ([](const QByteArray &array, char separator) -> QByteArray { \
        QString tmp = QString(array.toHex()); \
        QString res; \
        for (int i = 0; i < tmp.length(); i += 2) { \
            res.append(tmp.mid(i, 2)); \
            if (i + 2 < tmp.length() && separator != '\0') { \
                res.append(separator); \
            } \
        } \
        return res.toUtf8(); \
    })(ba, c)
#endif
