/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include "SAKUdpTransponder.hh"

SAKUdpTransponder::SAKUdpTransponder(QWidget *parent)
    :SAKTransponder(parent)
{

}

SAKUdpTransponder::SAKUdpTransponder(quint64 id, QWidget *parent)
    :SAKTransponder(id, parent)
{

}

QVariant SAKUdpTransponder::parametersContext()
{
    return QVariant::fromValue(this);
}

SAKDebuggerDevice *SAKUdpTransponder::device()
{
    return Q_NULLPTR;
}

void SAKUdpTransponder::onDeviceStateChanged(bool opened)
{
    Q_UNUSED(opened);
}
