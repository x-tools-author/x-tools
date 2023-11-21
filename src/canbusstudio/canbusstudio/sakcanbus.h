/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKCANBUS_H
#define SAKCANBUS_H

#include <QThread>

class SAKCanBus : public QThread
{
    Q_OBJECT
public:
    SAKCanBus(QObject *parent = nullptr);
    ~SAKCanBus();
};

#endif // SAKCANBUS_H
