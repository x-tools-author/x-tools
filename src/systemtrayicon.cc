/***************************************************************************************************
 * Copyright 2022-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "systemtrayicon.h"
#include <QAction>
#include <QDir>
#include <QMenu>

SystemTrayIcon::SystemTrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/resources/images/SAKLogo.png"));
    setToolTip(tr("Qt Swiss Army Knife"));

    QMenu *menu = new QMenu();
    menu->addAction(tr("Open main window"), this, [=]() { emit invokeShowMainWindow(); });
    menu->addSeparator();
    menu->addAction(tr("Exit program"), this, [=]() { emit invokeExit(); });
    setContextMenu(menu);
}

SystemTrayIcon::~SystemTrayIcon()
{
    this->hide();
    QMenu *menu = contextMenu();
    menu->deleteLater();
}
