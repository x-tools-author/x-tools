/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QMetaType>
#include <QSerialPortInfo>

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKDebuggerPluginTransponderBase.hh"
#include "SAKTransponderSerialPortItem.hh"
#include "ui_SAKDebuggerPluginTransponderSerialPort.h"

SAKTransponderSerialPortItem::SAKTransponderSerialPortItem(
        QWidget *parent
        )
    :SAKDebuggerPluginTransponderBase (parent)
    ,mUi(new Ui::SAKDebuggerPluginTransponderSerialPort)
{    
    mUi->setupUi(this);
}

SAKTransponderSerialPortItem::~SAKTransponderSerialPortItem()
{

}
