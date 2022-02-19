/******************************************************************************
 * Copyright 2022 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QDir>
#include <QMenu>
#include <QAction>
#include "SAKSystemTrayIcon.hh"

SAKSystemTrayIcon::SAKSystemTrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/resources/images/SAKLogo.png"));
    setToolTip(tr("Qt Swiss Army Knife"));

    QMenu *menu = new QMenu();
    menu->addAction(tr("Open main window"), this, [=](){emit invokeShowMainWindow();});
    menu->addSeparator();
    menu->addAction(tr("Exit program"), this, [=](){emit invokeExit();});
    setContextMenu(menu);
}

SAKSystemTrayIcon::~SAKSystemTrayIcon()
{
    this->hide();
    QMenu *menu = contextMenu();
    menu->deleteLater();
}
