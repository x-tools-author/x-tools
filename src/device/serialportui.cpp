/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "serialportui.h"
#include "ui_serialportui.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "common/xtools.h"
#include "serialport.h"
#include "utilities/serialportscanner.h"

SerialPortUi::SerialPortUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::SerialPortUi)
{
    ui->setupUi(this);
#if defined(Q_OS_LINUX)
    ui->comboBoxPortName->setEditable(true);
#endif

    m_scanner = new SerialPortScanner(this);
    onDevicesChanged(m_scanner->deviceInfos());
    connect(m_scanner, &SerialPortScanner::devicesChanged, this, &SerialPortUi::onDevicesChanged);
    m_scanner->start();

    connect(ui->comboBoxPortName,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &SerialPortUi::onDeviceIndexChanged);
    connect(ui->checkBoxIgnoredBusyDevices, &QCheckBox::clicked, this, [=](bool checked) {
        m_scanner->setIsBusyDevicesIgnored(checked);
    });

    setupBaudRate(ui->comboBoxBaudRate);
    setupDataBits(ui->comboBoxDataBits);
    setupParity(ui->comboBoxParity);
    setupStopBits(ui->comboBoxStopBits);
    setupFlowControl(ui->comboBoxFlowControl);

    onDeviceIndexChanged(0);
}

SerialPortUi::~SerialPortUi()
{
    if (m_scanner->isRunning()) {
        m_scanner->quit();
        m_scanner->wait();
    }

    delete ui;
}

QVariantMap SerialPortUi::save() const
{
    QVariantMap map;
    SerialPortItemKeys keys;
    map[keys.portName] = ui->comboBoxPortName->currentText();
    map[keys.baudRate] = ui->comboBoxBaudRate->currentText().toInt();
    map[keys.dataBits] = ui->comboBoxDataBits->currentData().toInt();
    map[keys.parity] = ui->comboBoxParity->currentData().toInt();
    map[keys.stopBits] = ui->comboBoxStopBits->currentData().toInt();
    map[keys.flowControl] = ui->comboBoxFlowControl->currentData().toInt();
    map[keys.ignoredBusyDevices] = ui->checkBoxIgnoredBusyDevices->isChecked();
    map[keys.optimizedFrame] = ui->checkBoxOptimizedFrame->isChecked();
    return map;
}

void SerialPortUi::load(const QVariantMap &map)
{
    if (map.isEmpty()) {
        return;
    }

    SerialPortItemKeys keys;
    QString portName = map.value(keys.portName).toString();
    int baudRate = map.value(keys.baudRate, 9600).toInt();
    int dataBits = map.value(keys.dataBits, static_cast<int>(QSerialPort::Data8)).toInt();
    int parity = map.value(keys.parity, static_cast<int>(QSerialPort::NoParity)).toInt();
    int stopBits = map.value(keys.stopBits, static_cast<int>(QSerialPort::OneStop)).toInt();
    int fc = map.value(keys.flowControl, static_cast<int>(QSerialPort::NoFlowControl)).toInt();
    bool ignoredBusyDevices = map.value(keys.ignoredBusyDevices, false).toBool();
    bool optimizedFrame = map.value(keys.optimizedFrame, false).toBool();

    m_scanner->setIsBusyDevicesIgnored(ignoredBusyDevices);

    ui->comboBoxPortName->setCurrentText(portName);
    ui->comboBoxBaudRate->setCurrentText(QString::number(baudRate));
    ui->comboBoxDataBits->setCurrentIndex(ui->comboBoxDataBits->findData(dataBits));
    ui->comboBoxParity->setCurrentIndex(ui->comboBoxParity->findData(parity));
    ui->comboBoxStopBits->setCurrentIndex(ui->comboBoxStopBits->findData(stopBits));
    ui->comboBoxFlowControl->setCurrentIndex(ui->comboBoxFlowControl->findData(fc));
    ui->checkBoxIgnoredBusyDevices->setChecked(ignoredBusyDevices);
    ui->checkBoxOptimizedFrame->setChecked(optimizedFrame);
}

Device *SerialPortUi::newDevice()
{
    return new SerialPort(this);
}

void SerialPortUi::refresh()
{
    setupPortName(ui->comboBoxPortName);
}

void SerialPortUi::onPortNameChanged(const QStringList &portName)
{
    if (!ui->comboBoxPortName->isEnabled()) {
        return;
    }

    QStringList items;
    for (int i = 0; i < ui->comboBoxPortName->count(); i++) {
        items.append(ui->comboBoxPortName->itemText(i));
    }

    if (items == portName) {
        return;
    }

    QString currentPortName = ui->comboBoxPortName->currentText();
    ui->comboBoxPortName->clear();
    ui->comboBoxPortName->addItems(portName);
    ui->comboBoxPortName->setCurrentText(currentPortName);
}

void SerialPortUi::onDevicesChanged(const QList<QSerialPortInfo> &devices)
{
    if (!ui->comboBoxPortName->isEnabled()) {
        return;
    }

    if (devices.isEmpty()) {
        ui->comboBoxPortName->clear();
        return;
    }

    const QString tmp = ui->comboBoxPortName->currentText();
    ui->comboBoxPortName->clear();
    for (const QSerialPortInfo &info : devices) {
        ui->comboBoxPortName->addItem(info.portName());
        const int index = ui->comboBoxPortName->count() - 1;
        ui->comboBoxPortName->setItemData(index, info.description(), Qt::ToolTipRole);
    }

    int index = ui->comboBoxPortName->findText(tmp);
    index = index < 0 ? 0 : index;
    ui->comboBoxPortName->setCurrentIndex(index);
}

void SerialPortUi::onDeviceIndexChanged(int index)
{
    if (index < 0 || index >= ui->comboBoxPortName->count()) {
        return;
    }

    const QString tips = ui->comboBoxPortName->itemData(index, Qt::ToolTipRole).toString();
    ui->comboBoxPortName->setToolTip(tips);
}