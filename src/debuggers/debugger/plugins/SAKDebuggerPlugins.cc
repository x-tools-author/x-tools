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
#include <QDebug>

#include "SAKDebuggerPlugins.hh"

SAKDebuggerPlugins::SAKDebuggerPlugins(QPushButton *readmeBt,
                                       QPushButton *menuBt,
                                       QSettings *settings,
                                       const QString &settingsGroup,
                                       QObject *parent)
    :QObject(parent)
    ,mAutoResponse(Q_NULLPTR)
{
    QMenu *menu = new QMenu(menuBt);
    menuBt->setMenu(menu);

    // Instance plugins
    mAutoResponse = new SAKPluginAutoResponse();

    QMenu *embedMenu = new QMenu(tr("Inset to Center"), menu);
    menu->addMenu(embedMenu);
    embedMenu->addAction(tr("Cancel Inset"), this, [](){});
    embedMenu->addSeparator();
    embedMenu->addAction(tr("Data Transmission"), this, [](){});
    embedMenu->addAction(tr("Auto Response"), this, &SAKDebuggerPlugins::showAutoResponsePlugin);
    embedMenu->addAction(tr("Charts"), this, [](){});
    embedMenu->addAction(tr("3D"), this, [](){});
    menu->addSeparator();
    menu->addAction(tr("Data Transmission"), this, [](){});
    menu->addAction(tr("Auto Response"), this, &SAKDebuggerPlugins::showAutoResponsePlugin);
    menu->addAction(tr("Charts"), this, [](){});
    menu->addAction(tr("3D"), this, [](){});
    menu->addSeparator();
    menu->addAction(tr("Auto Reload"), this, [](){});
    menu->addAction(tr("Reload All"), this, [](){});
}

SAKDebuggerPlugins::~SAKDebuggerPlugins()
{
    mAutoResponse->deleteLater();
}

void SAKDebuggerPlugins::showAutoResponsePlugin()
{
    if (mAutoResponse->isHidden()) {
        mAutoResponse->show();
    } else {
        mAutoResponse->activateWindow();
    }
}
