/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QToolBar>
#include "SAKDebuggerPlugin.hh"

SAKDebuggerPlugin::SAKDebuggerPlugin(QObject *parent)
    :QObject(parent)
{

}

SAKDebuggerPlugin::~SAKDebuggerPlugin()
{

}

const QString SAKDebuggerPlugin::qtVersion()
{
    QString majorVersion = QString::number(QT_VERSION_MAJOR);
    QString minorVersion = QString::number(QT_VERSION_MINOR);
    QString patchVersion = QString::number(QT_VERSION_PATCH);

    return QString("%1.%2.%3").arg(majorVersion, minorVersion, patchVersion);
}
