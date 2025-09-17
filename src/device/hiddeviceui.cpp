/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hiddeviceui.h"
#include "ui_hiddeviceui.h"

#include <QSerialPort>
#include <QSerialPortInfo>

#include "common/xtools.h"
#include "serialport.h"
#include "utilities/hidscanner.h"
#include "utilities/serialportscanner.h"

HidDeviceUi::HidDeviceUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::HidDeviceUi)
{
    ui->setupUi(this);

    m_hidScanner = new HidScanner(this);
    auto infos = m_hidScanner->hidInfos();
    onDevicesChanged(infos);
    connect(m_hidScanner, &HidScanner::devicesChanged, this, &HidDeviceUi::onDevicesChanged);

    connect(ui->comboBoxPortHid,
            qOverload<int>(&QComboBox::currentIndexChanged),
            this,
            &HidDeviceUi::onDeviceIndexChanged);
    connect(ui->checkBoxIgnoredBusyDevices, &QCheckBox::clicked, this, [=](bool checked) {
        m_hidScanner->setIsBusyDevicesIgnored(checked);
    });

    m_hidScanner->start();
}

HidDeviceUi::~HidDeviceUi()
{
    if (m_hidScanner->isRunning()) {
        m_hidScanner->quit();
        m_hidScanner->wait();
    }

    delete ui;
}

QVariantMap HidDeviceUi::save() const
{
    QVariantMap map;

    return map;
}

void HidDeviceUi::load(const QVariantMap &map)
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

    m_hidScanner->setIsBusyDevicesIgnored(ignoredBusyDevices);
}

Device *HidDeviceUi::newDevice()
{
    return new SerialPort(this);
}

void HidDeviceUi::refresh()
{
    setupPortName(ui->comboBoxPortHid);
}

void HidDeviceUi::onPortNameChanged(const QStringList &portName)
{
    if (!ui->comboBoxPortHid->isEnabled()) {
        return;
    }

    QStringList items;
    for (int i = 0; i < ui->comboBoxPortHid->count(); i++) {
        items.append(ui->comboBoxPortHid->itemText(i));
    }

    if (items == portName) {
        return;
    }

    QString currentPortName = ui->comboBoxPortHid->currentText();
    ui->comboBoxPortHid->clear();
    ui->comboBoxPortHid->addItems(portName);
    ui->comboBoxPortHid->setCurrentText(currentPortName);
}

QString unsignedShortToString(unsigned short v)
{
    QString str = QString::number(v, 16).toUpper();
    if (v < 256) {
        while (str.length() < 2) {
            str = "0" + str;
        }
    } else {
        while (str.length() < 4) {
            str = "0" + str;
        }
    }

    return "0x" + str;
}

void HidDeviceUi::onDevicesChanged(const QList<HidDeviceInfo> &infos)
{
    if (!ui->comboBoxPortHid->isEnabled()) {
        return;
    }

    QString tmp = ui->comboBoxPortHid->currentText();
    ui->comboBoxPortHid->clear();
    for (const auto &info : infos) {
        ui->comboBoxPortHid->addItem(info.productString, info.path);

        QString tooltip = tr("Path") + ": " + info.path + "\n";
        tooltip += tr("Vendor ID") + ": " + unsignedShortToString(info.vendorId) + "\n";
        tooltip += tr("Product ID") + ": " + unsignedShortToString(info.productId) + "\n";
        tooltip += tr("Serial Number") + ": " + info.serialNumber + "\n";
        tooltip += tr("Release Number") + ": " + QString::number(info.releaseNumber) + "\n";
        tooltip += tr("Manufacturer") + ": " + info.manufacturerString + "\n";
        tooltip += tr("Product") + ": " + info.productString + "\n";
        tooltip += tr("Usage Page") + ": " + unsignedShortToString(info.usagePage) + "\n";
        tooltip += tr("Usage") + ": " + unsignedShortToString(info.usage) + "\n";
        tooltip += tr("Interface Number") + ": " + QString::number(info.interfaceNumber) + "\n";
        tooltip += tr("Bus Type") + ": " + unsignedShortToString(info.busType);

        ui->comboBoxPortHid->setItemData(ui->comboBoxPortHid->count() - 1, tooltip, Qt::ToolTipRole);
    }

    int index = ui->comboBoxPortHid->findText(tmp);
    if (index >= 0) {
        ui->comboBoxPortHid->setCurrentIndex(index);
        onDeviceIndexChanged(index);
    } else {
        if (ui->comboBoxPortHid->count() > 0) {
            onDeviceIndexChanged(ui->comboBoxPortHid->currentIndex());
        }
    }
}

void HidDeviceUi::onDeviceIndexChanged(int index)
{
    QString ts = ui->comboBoxPortHid->itemData(index, Qt::ToolTipRole).toString();
    ui->comboBoxPortHid->setToolTip(ts);
}