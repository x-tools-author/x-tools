/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMenu>
#include "SAKDebuggerPlugins.hh"

SAKDebuggerPlugins::SAKDebuggerPlugins(QPushButton *readmeBt,
                                       QPushButton *menuBt,
                                       QSettings *settings,
                                       const QString &settingsGroup,
                                       QObject *parent)
    :QObject(parent)
{
    QMenu *menu = new QMenu(menuBt);
    menuBt->setMenu(menu);

    QMenu *embedMenu = new QMenu(tr("Inset to Center"), menu);
    menu->addMenu(embedMenu);
    embedMenu->addAction(tr("Cancel Inset"), this, [](){});
    embedMenu->addSeparator();
    embedMenu->addAction(tr("Data Transmission"), this, [](){});
    embedMenu->addAction(tr("Auto Response"), this, [](){});
    embedMenu->addAction(tr("Charts"), this, [](){});
    embedMenu->addAction(tr("3D"), this, [](){});
    menu->addSeparator();
    menu->addAction(tr("Data Transmission"), this, [](){});
    menu->addAction(tr("Auto Response"), this, [](){});
    menu->addAction(tr("Charts"), this, [](){});
    menu->addAction(tr("3D"), this, [](){});
    menu->addSeparator();
    menu->addAction(tr("Auto Reload"), this, [](){});
    menu->addAction(tr("Reload All"), this, [](){});
}
