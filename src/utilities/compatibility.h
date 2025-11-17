/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtVersionChecks>

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
#define xWebSocketErrorOccurred &QWebSocket::errorOccurred
#else
#define xWebSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QWebSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xLocalSocketErrorOccurred &QLocalSocket::errorOccurred
#else
#define xLocalSocketErrorOccurred qOverload<QLocalSocket::LocalSocketError>(&QLocalSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xTcpSocketErrorOccurred &QTcpSocket::errorOccurred
#else
#define xTcpSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QTcpSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define xUdpSocketErrorOccurred &QUdpSocket::errorOccurred
#else
#define xUdpSocketErrorOccurred qOverload<QAbstractSocket::SocketError>(&QUdpSocket::error)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#define xComboBoxActivated &QComboBox::activated
#else
#define xComboBoxActivated qOverload<int>(&QComboBox::activated)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 2, 0)
#define xComboBoxIndexChanged &QComboBox::currentIndexChanged
#else
#define xComboBoxIndexChanged qOverload<int>(&QComboBox::currentIndexChanged)
#endif

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#define xDefaultStyleName Application::style()->name()
#else
#define xDefaultStyleName QString("fusion")
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
