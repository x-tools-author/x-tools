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
#include <QModbusDataUnit>

#include "SAKModbusDebugPage.hh"
#include "SAKModbusCommonRegister.hh"
#include "SAKModbusCommonFlowLayout.hh"
#include "SAKModbusCommonController.hh"
#include "SAKModbusCommonRegisterView.hh"
#include "SAKModbusClientControllerTcp.hh"
#include "SAKModbusServerControllerTcp.hh"
#include "SAKModbusCommonSerialPortSection.hh"
#include "SAKModbusClientControllerSerialPort.hh"
#include "SAKModbusServerControllerSerialPort.hh"
#include "SAKModbusCommonRegisterViewController.hh"

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
    ui->disconnectionPushButton->setEnabled(false);

    // Add a button to tab bar
    mMenuPushButton = new QPushButton(QString("..."), this);
    ui->tabWidget->setCornerWidget(mMenuPushButton);

    // Tab page information
    struct PageInfo {QWidget *widget; QModbusDataUnit::RegisterType type;};
    QList<PageInfo> pageInfoList;
    pageInfoList << PageInfo{ui->coilsWidget, QModbusDataUnit::Coils}
                 << PageInfo{ui->discreteInputsWidget, QModbusDataUnit::DiscreteInputs}
                 << PageInfo{ui->holdingRegistersWidget, QModbusDataUnit::HoldingRegisters}
                 << PageInfo{ui->inputRegistersWidget, QModbusDataUnit::InputRegisters};

    // Add page to tab widget
    for (auto var : pageInfoList){
        var.widget->setContentsMargins(0, 0, 0, 0);
        if (!var.widget->layout()){
            QVBoxLayout *vLayout = new QVBoxLayout;
            vLayout->setMargin(0);
            var.widget->setLayout(vLayout);
        }
        auto registerView = new SAKModbusCommonRegisterView(var.type);
        mRegisterViewList.append(registerView);
        registerView->setContentsMargins(0, 0, 0, 0);
        mRegisterViewController = new SAKModbusCommonRegisterViewController;
        connect(mRegisterViewController, &SAKModbusCommonRegisterViewController::invokeUpdateRegister, registerView, &SAKModbusCommonRegisterView::updateRegister);
        connect(registerView, &SAKModbusCommonRegisterView::registerValueChanged, this, &SAKModbusDebugPage::setData);
        connect(registerView, &SAKModbusCommonRegisterView::invokeUpdateRegisterValue, this, &SAKModbusDebugPage::updateRegisterValue);
        var.widget->layout()->addWidget(registerView);
        var.widget->layout()->addWidget(mRegisterViewController);
    }

    // Combo box items
    struct DeviceInfo {int type; QString name;};
    QList<DeviceInfo> deviceInfoList;
    deviceInfoList << DeviceInfo{TcpClient, tr("Tcp Client")}
                   << DeviceInfo{TcpServer, tr("Tcp Server")}
                   << DeviceInfo{SerialPortClient, tr("SerialPort Client")}
                   << DeviceInfo{SerialPortServer, tr("SerialPort Server")};

    // Add items to combo box
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

void SAKModbusDebugPage::outputModbusDataUnit(QModbusDataUnit mdu)
{
    auto startAddress = mdu.startAddress();
    auto valueCount = mdu.valueCount();
    QList<qint16> valueList;
    for (unsigned int i = startAddress; i < valueCount; i++){
        auto value = mdu.value(i);
        valueList.append(value);
    }

    QByteArray data;
    for (auto var : valueList){
        data.append(reinterpret_cast<char*>(&var)[1]);
        data.append(reinterpret_cast<char*>(&var)[0]);
    }

    ui->textBrowser->append(QString(data.toHex(' ')));
}

void SAKModbusDebugPage::setData(QModbusDataUnit::RegisterType type, quint16 address, quint16 value)
{
    if (mController){
        mController->setData(type, address, value);
    }
}

void SAKModbusDebugPage::updateRegisterValue(QModbusDataUnit::RegisterType registerTyp, quint16 startAddress, quint16 addressNumber)
{
    auto view = registerView(registerTyp);
    if (view){
        for (quint16 i = startAddress; i < addressNumber; i++){
            auto value = mController->registerValue(registerTyp, startAddress + i);
            view->updateRegisterValue(startAddress + i, value);
        }
    }
}

SAKModbusCommonRegisterView *SAKModbusDebugPage::registerView(QModbusDataUnit::RegisterType registerTyp)
{
    for (auto view : mRegisterViewList){
        if (view->registerType() == registerTyp){
            return view;
        }
    }

    return Q_NULLPTR;
}

void SAKModbusDebugPage::dataWritten(QModbusDataUnit::RegisterType table, int address, int size)
{
    // The operation is for client only.
    auto server = qobject_cast<QModbusServer*>(mController->device());
    if (server){
        for (int i = address; i < size; i++){
            quint16 value = 0;
            server->data(table, address + i, &value);
            auto view = registerView(table);
            view->updateRegisterValue(address+i, value);
        }
    }

    // The operation is for client only.
    auto clientController = qobject_cast<SAKModbusClientController*>(mController);
    if (clientController){
        for (int i = address; i < size; i++){
            quint16 value = 0;
            clientController->tempData(table, address + i, &value);
            auto view = registerView(table);
            view->updateRegisterValue(address+i, value);
        }
    }
}

void SAKModbusDebugPage::on_deviceTypeComboBox_currentIndexChanged(int index)
{
    mController = qobject_cast<SAKModbusCommonController*>(controllerFromType(index));
    mController->setContentsMargins(0, 0, 0 ,0);

    auto *dev = mController->device();
    connect(mController, &SAKModbusCommonController::modbusDataUnitRead, this, &SAKModbusDebugPage::outputModbusDataUnit);
    connect(mController, &SAKModbusCommonController::modbusDataUnitWritten, this, &SAKModbusDebugPage::outputModbusDataUnit);
    connect(mController, &SAKModbusCommonController::dataWritten, this, &SAKModbusDebugPage::dataWritten);
#if 0
    // The wrong way to call a vitual function.
    connect(mRegisterViewController, &SAKModbusCommonRegisterViewController::invokeExport, mController, &SAKModbusCommonController::exportRegisterData);
    connect(mRegisterViewController, &SAKModbusCommonRegisterViewController::invokeImport, mController, &SAKModbusCommonController::importRegisterData);
#else
    connect(mRegisterViewController, &SAKModbusCommonRegisterViewController::invokeExport, this, [=](){mController->exportRegisterData();});
    connect(mRegisterViewController, &SAKModbusCommonRegisterViewController::invokeImport, this, [=](){mController->importRegisterData();});
#endif
    connect(dev, &QModbusDevice::stateChanged, this, [=](){
        ui->connectionPushButton->setEnabled(dev->state() == QModbusDevice::UnconnectedState);
        ui->disconnectionPushButton->setEnabled(dev->state() == QModbusDevice::ConnectedState);
        ui->deviceTypeComboBox->setEnabled(dev->state() == QModbusDevice::UnconnectedState);
    }, Qt::QueuedConnection);

    QLayout *hLayout = ui->deviceControllerWidget->layout();
    if (hLayout){
        // Remove the old pannel
        QLayoutItem *item = hLayout->takeAt(0);
        if (item){
            delete item->widget();
            delete item;
        }

        // Add a new pannel
        hLayout->addWidget(mController);
        ui->connectionPushButton->setEnabled(true);
    }

    // Update the value of registers.
    quint16 startAddress = 0;
    quint16 registerNumber = 16;
    for (auto var : mRegisterViewList){
        var->updateRegister(startAddress, registerNumber);
        for (int i = startAddress; i < registerNumber; i++){
            var->updateRegisterValue(startAddress + i, mController->registerValue(var->registerType(), startAddress + i));
        }
    }
}

void SAKModbusDebugPage::on_connectionPushButton_clicked()
{
    ui->connectionPushButton->setEnabled(false);
    ui->disconnectionPushButton->setEnabled(false);
    mController->open();
}

void SAKModbusDebugPage::on_disconnectionPushButton_clicked()
{
    ui->connectionPushButton->setEnabled(false);
    ui->disconnectionPushButton->setEnabled(false);
    mController->closeDevice();
}
