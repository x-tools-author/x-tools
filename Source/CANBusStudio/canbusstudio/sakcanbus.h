/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
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
