/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKCOMPATIBILITY_H
#define SAKCOMPATIBILITY_H

#include <QtGlobal>

#if QT_VERSION >= QT_VERSION_CHECK(5, 15, 0)
#define SAK_SOCKET_ERROR &QAbstractSocket::errorOccurred
#else
#define SAK_SOCKET_ERROR static_cast<void(QAbstractSocket::*)\
(QAbstractSocket::SocketError)>(&QAbstractSocket::error)
#endif

#endif // SAKCOMPATIBILITY_H
