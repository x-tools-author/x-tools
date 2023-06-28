/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBASEASSISTANT_HH
#define SAKBASEASSISTANT_HH

#include <QThread>
#include <QLoggingCategory>

class SAKBaseAssistant : public QThread
{
    Q_OBJECT
public:
    explicit SAKBaseAssistant(QObject *parent = nullptr);
};

#endif // SAKBASEASSISTANT_HH
