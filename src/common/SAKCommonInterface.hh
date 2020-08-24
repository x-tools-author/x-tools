/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCOMMONINTERFACE_HH
#define SAKCOMMONINTERFACE_HH

#include <QObject>

/// @brief This class provides interfaces to replace the functions introduced in the new version of Qt
/// so that the old version of Qt can also compile the project
class SAKCommonInterface:public QObject
{
    Q_OBJECT
public:
    SAKCommonInterface(QObject *parent = Q_NULLPTR);

    QByteArray byteArrayToHex(QByteArray &array, char separator);
};

#endif
