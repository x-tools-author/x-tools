/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKMODBUS_HH
#define SAKMODBUS_HH

#include <QThread>

class SAKModbus : public QThread
{
    Q_OBJECT
public:
    SAKModbus(QObject *parent = nullptr);
};

#endif // SAKMODBUS_HH
