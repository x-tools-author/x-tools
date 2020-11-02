/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QLineEdit>

#include "SAKModbusDebugPage.hh"
#include "SAKModbusCommonRegister.hh"
#include "SAKModbusCommonFlowLayout.hh"
#include "SAKModbusCommonReigsterView.hh"
#include "SAKModbusClientControllerTcp.hh"
#include "SAKModbusServerControllerTcp.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"
#include "SAKModbusServerControllerSerialPort.hh"

#include "ui_SAKModbusDebugPage.h"

SAKModbusDebugPage::SAKModbusDebugPage(int type, QString name, QSettings *settings, QSplashScreen *splashScreen, QSqlDatabase *sqlDatabase, QWidget *parent)
    :QWidget(parent)
    ,mType(type)
    ,mName(name)
    ,mSettings(settings)
    ,mSplashScreen(splashScreen)
    ,mSqlDatabase(sqlDatabase)
    ,ui(new Ui::SAKModbusDebugPage)
{
    ui->setupUi(this);
    ui->deviceControllerWidget->setContentsMargins(0, 0, 0, 0);
    ui->deviceControllerWidget->setLayout(new QVBoxLayout);
    ui->deviceControllerWidget->layout()->setContentsMargins(0, 0, 0, 0);

    // Add a button to tab bar
    mMenuPushButton = new QPushButton(QString("..."), this);
    ui->tabWidget->setCornerWidget(mMenuPushButton);

    QList<QWidget*> registerViewList;
    registerViewList << ui->coilsWidget
                     << ui->discreteInputsWidget
                     << ui->holdingRegistersWidget
                     << ui->inputRegistersWidget;

    for (auto var : registerViewList){
        if (!var->layout()){
            var->setLayout(new QHBoxLayout);
        }
        auto registerView = new SAKModbusCommonReigsterView;
        var->layout()->addWidget(registerView);
        for (int i = 0; i < 20; i++){
            registerView->addWidget(new SAKModbusCommonRegister);
        }
    }

    struct DeviceInfo {
        int type;
        QString name;
    };
    QList<DeviceInfo> deviceInfoList;
    deviceInfoList << DeviceInfo{TcpClient, tr("Tcp Client")}
                   << DeviceInfo{TcpServer, tr("Tcp Server")}
                   << DeviceInfo{SerialPortClient, tr("SerialPort Client")}
                   << DeviceInfo{SerialPortServer, tr("SerialPort Server")};
    for (auto var : deviceInfoList){
        ui->deviceTypeComboBox->addItem(var.name, QVariant::fromValue(var.type));
    }
}

SAKModbusDebugPage::~SAKModbusDebugPage()
{
    delete ui;
}

QWidget *SAKModbusDebugPage::controllerFromType(int type)
{
    QWidget *w = Q_NULLPTR;
    switch (type) {
    case TcpClient:
        w = new SAKModbusClientControllerTcp(this);
        break;
    case TcpServer:
        w = new SAKModbusServerControllerTcp(this);
        break;
    case SerialPortClient:
        w = new SAKModbusClientControllerSerialPort(this);
        break;
    case SerialPortServer:
        w = new SAKModbusServerControllerSerialPort(this);
        break;
    default:
        Q_ASSERT_X(false, __FUNCTION__, "Unknow device type!");
        break;
    }

    return w;
}

void SAKModbusDebugPage::on_deviceTypeComboBox_currentIndexChanged(int index)
{
    QWidget *w = controllerFromType(index);
    w->setContentsMargins(0, 0, 0 ,0);
    QLayout *hLayout = ui->deviceControllerWidget->layout();

    if (hLayout){
        // Remove the old pannel
        QLayoutItem *item = hLayout->takeAt(0);
        if (item){
            delete item->widget();
            delete item;
        }

        // Add a new pannel
        hLayout->addWidget(w);
    }
}
