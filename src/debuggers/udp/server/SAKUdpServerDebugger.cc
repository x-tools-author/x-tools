/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QWidget>
#include <QHBoxLayout>

#include "SAKUdpServerDevice.hh"
#include "SAKUdpServerDebugger.hh"
#include "SAKUdpServerMulticast.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKUdpServerController.hh"

SAKUdpServerDebugger::SAKUdpServerDebugger(QSettings *settings,
                                           const QString settingsGroup,
                                           QSqlDatabase *sqlDatabase,
                                           QWidget *parent)
    :SAKDebugger(settings, settingsGroup, sqlDatabase, parent)
{
    mController = new SAKUdpServerController(settings, settingsGroup, parent);
    mDevice = new SAKUdpServerDevice(settings, settingsGroup, this, parent);
    mMulticast = new SAKUdpServerMulticast(settings, settingsGroup, this);
    mDevice->setMulticastParameters(mMulticast->parameters());

    connect(mDevice, &SAKUdpServerDevice::addClient,
            mController, &SAKUdpServerController::onAddClient);
    connect(mMulticast, &SAKUdpServerMulticast::parametersChanged,
            mDevice, &SAKUdpServerDevice::setMulticastParameters);

    initDebugger();
}

SAKDebuggerDevice* SAKUdpServerDebugger::device()
{
    return mDevice;
}

SAKDebuggerController *SAKUdpServerDebugger::controller()
{
    return mController;
}

void SAKUdpServerDebugger::addActionToDeviceMenu(QMenu *menu)
{
    menu->addAction(tr("Multicast Editor"), mMulticast,
                    &SAKUdpServerMulticast::show);
}

void SAKUdpServerDebugger::updateUiState(bool opened)
{
    mMulticast->updateUiState(opened);
}
