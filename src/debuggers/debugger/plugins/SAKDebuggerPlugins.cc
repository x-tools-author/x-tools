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

    QMenu *embedMenu = new QMenu(tr("嵌入插件"), menu);
    menu->addMenu(embedMenu);
    embedMenu->addAction(tr("取消嵌入"), this, [](){});
    embedMenu->addSeparator();
    embedMenu->addAction(tr("数据转发"), this, [](){});
    embedMenu->addAction(tr("自动应答"), this, [](){});
    embedMenu->addAction(tr("数据图表"), this, [](){});
    embedMenu->addAction(tr("三维展示"), this, [](){});
    menu->addSeparator();
    menu->addAction(tr("数据转发"), this, [](){});
    menu->addAction(tr("自动应答"), this, [](){});
    menu->addAction(tr("数据图表"), this, [](){});
    menu->addAction(tr("三维展示"), this, [](){});
    menu->addSeparator();
    menu->addAction(tr("自动重载"), this, [](){});
    menu->addAction(tr("全部重载"), this, [](){});
}
