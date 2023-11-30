/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKTOOLUIFACTORY_H
#define SAKTOOLUIFACTORY_H

#include <QObject>

class SAKBaseToolUi;
class SAKToolUiFactory : public QObject
{
    Q_OBJECT
private:
    explicit SAKToolUiFactory(QObject *parent = nullptr);

public:
    static SAKToolUiFactory *instance();
    SAKBaseToolUi *createToolUi(int type);
};

#endif // SAKTOOLUIFACTORY_H
