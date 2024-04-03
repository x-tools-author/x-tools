/***************************************************************************************************
 * Copyright 2022-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "SystemTrayIcon.h"

#include <QAction>
#include <QDir>
#include <QMenu>

SystemTrayIcon::SystemTrayIcon(QObject *parent)
    : QSystemTrayIcon(parent)
{
    setIcon(QIcon(":/Resources/Images/Logo.png"));
    setToolTip(tr("xTools"));

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
