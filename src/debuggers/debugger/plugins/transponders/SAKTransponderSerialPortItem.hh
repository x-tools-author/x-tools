/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
****************************************************************************************/
#ifndef SAKDEBUGGERPLUGINTRANSPONDERSERIALPORT_HH
#define SAKDEBUGGERPLUGINTRANSPONDERSERIALPORT_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>
#include <QIODevice>
#include <QPushButton>
#include <QSerialPort>

#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginTransponderBase.hh"

namespace Ui {
    class SAKDebuggerPluginTransponderSerialPort;
}

class SAKTransponderSerialPortItem : public SAKDebuggerPluginTransponderBase
{
    Q_OBJECT
public:
    SAKTransponderSerialPortItem(QWidget *parent = Q_NULLPTR);
     ~SAKTransponderSerialPortItem();


private:
    Ui::SAKDebuggerPluginTransponderSerialPort *mUi;
};

#endif
