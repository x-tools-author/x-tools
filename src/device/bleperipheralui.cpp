/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "bleperipheralui.h"
#include "ui_bleperipheralui.h"

#include <QLowEnergyService>
#include <QMessageBox>

#include "bleperipheral.h"
#include "common/xtools.h"
#include "utilities/blescanner.h"
#include "utilities/compatibility.h"

BlePeripheralUi::BlePeripheralUi(QWidget *parent)
    : DeviceUi(parent)
    , ui(new Ui::BlePeripheralUi)
{
    ui->setupUi(this);
    // clang-format off
    connect(ui->comboBoxServices, xComboBoxIndexChanged, this, &BlePeripheralUi::onServiceIndexChanged);
    connect(ui->comboBoxCharacteristics, xComboBoxIndexChanged, this, &BlePeripheralUi::onCharacteristicIndexChanged);
    connect(ui->pushButtonNotify, &QPushButton::clicked, this, &BlePeripheralUi::onNotifyButtonClicked);
    connect(ui->pushButtonRead, &QPushButton::clicked, this, &BlePeripheralUi::onReadButtonClicked);
    // clang-format on

    m_scanner = new BleScanner(this);
    connect(ui->pushButtonScan, &QPushButton::clicked, this, &BlePeripheralUi::onScanButtonClicked);
    connect(m_scanner, &BleScanner::started, this, &BlePeripheralUi::onScanningStarted);
    connect(m_scanner, &BleScanner::finished, this, &BlePeripheralUi::onScanningFinished);
    connect(m_scanner, &BleScanner::deviceDiscovered, this, &BlePeripheralUi::onDeviceDiscovered);
    connect(m_scanner, &BleScanner::errorOccurred, this, &BlePeripheralUi::onErrorOccurred);

    ui->progressBarOpening->hide();
    ui->progressBarScanning->hide();
    ui->labelUnsupported->hide();
    ui->labelWriteMode->hide();
    ui->comboBoxWriteMode->hide();
    ui->pushButtonRead->hide();
    ui->pushButtonNotify->hide();
    ui->pushButtonScan->setEnabled(true);
}

BlePeripheralUi::~BlePeripheralUi()
{
    delete ui;
}

Device *BlePeripheralUi::newDevice()
{
    BlePeripheral *device = new BlePeripheral(this);
    ui->comboBoxServices->clear();
    ui->comboBoxCharacteristics->clear();
    connect(device, &BlePeripheral::started, this, [this]() { ui->progressBarOpening->show(); });
    connect(device, &BlePeripheral::discoveryFinished, this, [this]() {
        ui->progressBarOpening->hide();
    });
    connect(device, &BlePeripheral::serviceDiscovered, this, [this](QLowEnergyService *service) {
        setupLowEnergyService(service);
        ui->comboBoxServices->addItem(service->serviceName(), QVariant::fromValue(service));
    });

    return device;
}

QVariantMap BlePeripheralUi::save() const
{
    QVariantMap map;
    map["deviceInfo"] = ui->comboBoxDevices->currentData();
    map["service"] = ui->comboBoxServices->currentData();
    map["writeMode"] = ui->comboBoxWriteMode->currentData();
    map["characteristic"] = ui->comboBoxCharacteristics->currentData();
    return map;
}

void BlePeripheralUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void BlePeripheralUi::setUiEnabled(bool enabled)
{
    Q_UNUSED(enabled);
}

void BlePeripheralUi::updateNotifyText()
{
    QVariant serviceVar = ui->comboBoxServices->currentData();
    QVariant characteristicVar = ui->comboBoxCharacteristics->currentData();
    auto *service = serviceVar.value<QLowEnergyService *>();
    auto characteristic = characteristicVar.value<QLowEnergyCharacteristic>();

    if (!service || !characteristic.isValid()) {
        return;
    }

    auto type = QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration;
    auto descriptor = characteristic.descriptor(type);
    if (!descriptor.isValid()) {
        return;
    }

    setNotifyText(descriptor.value());
}

void BlePeripheralUi::resetVisible()
{
    ui->labelWriteMode->hide();
    ui->comboBoxWriteMode->hide();
    ui->pushButtonRead->hide();
    ui->pushButtonNotify->hide();
    ui->labelUnsupported->hide();
    ui->progressBarOpening->hide();
}

void BlePeripheralUi::setNotifyText(const QByteArray &value)
{
    if (value == notifyValue()) {
        ui->pushButtonNotify->setText(tr("Disnotify"));
    } else {
        ui->pushButtonNotify->setText(tr("Notify"));
    }
}

void BlePeripheralUi::setupWriteMode(bool enableWithResponse, bool writeWithoutResponse)
{
    ui->comboBoxWriteMode->clear();

    if (enableWithResponse) {
        auto withResponse = QLowEnergyService::WriteWithResponse;
        ui->comboBoxWriteMode->addItem(tr("With Response"), withResponse);
    }

    if (!writeWithoutResponse) {
        auto withoutResponse = QLowEnergyService::WriteWithoutResponse;
        ui->comboBoxWriteMode->addItem(tr("Without Response"), withoutResponse);
    }
}

void BlePeripheralUi::setupLowEnergyService(QLowEnergyService *service)
{
    if (!service) {
        qWarning() << "Service is null";
        return;
    }

    connect(service,
            &QLowEnergyService::descriptorWritten,
            this,
            &BlePeripheralUi::onDescriptorWritten,
            Qt::QueuedConnection);
    connect(service,
            &QLowEnergyService::descriptorRead,
            this,
            &BlePeripheralUi::onDescriptorRead,
            Qt::QueuedConnection);
}

void BlePeripheralUi::onScanButtonClicked()
{
    ui->pushButtonScan->setEnabled(false);

    if (m_scanner->isRunning()) {
        m_scanner->exit();
    } else {
        ui->comboBoxDevices->clear();
        m_scanner->start();
    }
}

void BlePeripheralUi::onServiceIndexChanged(int index)
{
    QVariant var = ui->comboBoxServices->itemData(index);
    if (!var.isValid()) {
        return;
    }

    QLowEnergyService *service = var.value<QLowEnergyService *>();
    if (!service) {
        return;
    }

    ui->comboBoxCharacteristics->clear();
    for (QLowEnergyCharacteristic &characteristic : service->characteristics()) {
        QVariant var = QVariant::fromValue(characteristic);
        QString name = characteristic.name();
        name = name.trimmed();
        if (name.isEmpty()) {
            name = tr("(No name)");
        }
        ui->comboBoxCharacteristics->addItem(name, var);
    }
}

void BlePeripheralUi::onCharacteristicIndexChanged(int index)
{
    if (!ui->comboBoxCharacteristics->count()) {
        resetVisible();
        return;
    }

    QVariant var = ui->comboBoxCharacteristics->itemData(index);
    if (!var.isValid()) {
        resetVisible();
        ui->labelUnsupported->show();
        return;
    }

    QLowEnergyCharacteristic characteristic = var.value<QLowEnergyCharacteristic>();
    if (!characteristic.isValid()) {
        resetVisible();
        ui->labelUnsupported->show();
        return;
    }

    auto properties = characteristic.properties();
    bool enableWithResponse = properties.testFlag(QLowEnergyCharacteristic::Write);
    bool writeWithoutResponse = properties.testFlag(QLowEnergyCharacteristic::WriteNoResponse);
    setupWriteMode(enableWithResponse, writeWithoutResponse);
    if (!(enableWithResponse || writeWithoutResponse)) {
        ui->labelWriteMode->hide();
        ui->comboBoxWriteMode->hide();
    } else {
        ui->labelWriteMode->show();
        ui->comboBoxWriteMode->show();
    }

    bool enableNotify = properties.testFlag(QLowEnergyCharacteristic::Notify);
    bool enableRead = properties.testFlag(QLowEnergyCharacteristic::Read);
    if (!(enableNotify || enableRead)) {
        ui->pushButtonNotify->hide();
        ui->pushButtonRead->hide();
    } else {
        ui->pushButtonNotify->show();
        ui->pushButtonRead->show();

        ui->pushButtonNotify->setEnabled(enableNotify);
        ui->pushButtonRead->setEnabled(enableRead);
    }

    if (enableNotify) {
        updateNotifyText();
    }

    if (!(enableNotify || enableRead || enableWithResponse || writeWithoutResponse)) {
        ui->labelUnsupported->show();
    } else {
        ui->labelUnsupported->hide();
    }
}

void BlePeripheralUi::onDescriptorWritten(const QLowEnergyDescriptor &info, const QByteArray &value)
{
    qInfo() << "Descriptor written" << info.name() << info.uuid();
    setNotifyText(value);
}

void BlePeripheralUi::onDescriptorRead(const QLowEnergyDescriptor &info, const QByteArray &value)
{
    qInfo() << "Descriptor read" << info.name() << info.uuid();
    setNotifyText(value);
}

void BlePeripheralUi::onNotifyButtonClicked()
{
    QVariant serviceVar = ui->comboBoxServices->currentData();
    QVariant characteristicVar = ui->comboBoxCharacteristics->currentData();
    auto *service = serviceVar.value<QLowEnergyService *>();
    auto characteristic = characteristicVar.value<QLowEnergyCharacteristic>();

    if (!service || !characteristic.isValid()) {
        qWarning("Invalid service or characteristic.");
        return;
    }

    auto type = QBluetoothUuid::DescriptorType::ClientCharacteristicConfiguration;
    auto descriptor = characteristic.descriptor(type);
    if (!descriptor.isValid()) {
        qWarning("Invalid descriptor");
        return;
    }

    QByteArray value = descriptor.value() == notifyValue() ? disnotifyValue() : notifyValue();
#if 1
    qInfo() << "Invoke write descriptor:" << descriptor.name() << value.toHex();
    service->writeDescriptor(descriptor, value);
#else
    auto descriptors = characteristic.descriptors();
    for (const auto &descriptor : descriptors) {
        qInfo() << "Invoke write descriptor:" << descriptor.name() << value.toHex();
        service->writeDescriptor(descriptor, value);
    }
#endif
}

void BlePeripheralUi::onReadButtonClicked()
{
    QVariant serviceVar = ui->comboBoxServices->currentData();
    QVariant characteristicVar = ui->comboBoxCharacteristics->currentData();
    auto *service = serviceVar.value<QLowEnergyService *>();
    auto characteristic = characteristicVar.value<QLowEnergyCharacteristic>();

    if (!service || !characteristic.isValid()) {
        qWarning("Invalid service or characteristic.");
        return;
    }

    service->readCharacteristic(characteristic);
}

void BlePeripheralUi::onScanningStarted()
{
    ui->pushButtonScan->setText(tr("Stop"));
    ui->pushButtonScan->setEnabled(true);
    ui->progressBarScanning->show();
}

void BlePeripheralUi::onScanningFinished()
{
    ui->pushButtonScan->setText(tr("Scan"));
    ui->pushButtonScan->setEnabled(true);
    ui->progressBarScanning->hide();
}

void BlePeripheralUi::onDeviceDiscovered(const QBluetoothDeviceInfo &info)
{
    this->ui->comboBoxDevices->addItem(info.name(), QVariant::fromValue(info));
}

void BlePeripheralUi::onErrorOccurred(const QString &errStr)
{
    QMessageBox::warning(this, tr("Error Occurred"), errStr);
}

QByteArray BlePeripheralUi::notifyValue() const
{
    static const QByteArray notifyValue = QByteArray::fromHex("0100");
    return notifyValue;
}

QByteArray BlePeripheralUi::disnotifyValue() const
{
    static const QByteArray disnotifyValue = QByteArray::fromHex("0000");
    return disnotifyValue;
}
