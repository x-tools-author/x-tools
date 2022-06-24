/******************************************************************************
 * Copyright 2022 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKOBJ_H
#define SAKOBJ_H

#include <QSettings>
#include <QByteArray>
#include <QLoggingCategory>

class SAKObj
{
public:
    SAKObj(const char *lc = "SAK.General");
protected:
    void printLog(QtMsgType type, const QString &log);
private:
    const QLoggingCategory mLoggingCategory;
};

#endif // SAKOBJ_H
