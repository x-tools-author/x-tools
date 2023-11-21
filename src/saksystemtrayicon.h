/***************************************************************************************************
 * Copyright 2022-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the
 * source code directory.
 **************************************************************************************************/
#ifndef SAKSYSTEMTRAYICON_H
#define SAKSYSTEMTRAYICON_H

#include <QSystemTrayIcon>

class SAKSystemTrayIcon : public QSystemTrayIcon
{
    Q_OBJECT
public:
    explicit SAKSystemTrayIcon(QObject *parent = Q_NULLPTR);
    ~SAKSystemTrayIcon();
signals:
    void invokeExit();
    void invokeShowMainWindow();
};

#endif // SAKSYSTEMTRAYICON_H
