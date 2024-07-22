/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "BleCentralUi.h"
#include "ui_BleCentralUi.h"

#include <QLowEnergyService>
#include <QMessageBox>

#include "../../IO/Communication/BleCentral.h"
#include "../../Unit/BleScanner.h"

BleCentralUi::BleCentralUi(xIO::DeviceType type, QWidget *parent)
    : DeviceUi(type, parent)
    , ui(new Ui::BleCentralUi)
{
    ui->setupUi(this);
    connect(ui->comboBoxServices,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &BleCentralUi::onServiceIndexChanged);
    connect(ui->comboBoxCharacteristics,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &BleCentralUi::onCharacteristicIndexChanged);
    connect(ui->pushButtonNotify, &QPushButton::clicked, this, &BleCentralUi::onNotifyButtonClicked);
    connect(ui->pushButtonRead, &QPushButton::clicked, this, &BleCentralUi::onReadButtonClicked);

    m_scanner = new BleScanner(this);
    connect(ui->pushButtonScan, &QPushButton::clicked, this, &BleCentralUi::onScanButtonClicked);
    connect(m_scanner, &BleScanner::started, this, [this]() {
        ui->pushButtonScan->setText(tr("Stop"));
        ui->pushButtonScan->setEnabled(true);
        ui->progressBarScanning->show();
    });
    connect(m_scanner, &BleScanner::finished, this, [this]() {
        ui->pushButtonScan->setText(tr("Scan"));
        ui->pushButtonScan->setEnabled(true);
        ui->progressBarScanning->hide();
    });
    connect(m_scanner, &BleScanner::deviceDiscovered, this, [this](const QBluetoothDeviceInfo &info) {
        this->ui->comboBoxDevices->addItem(info.name(), QVariant::fromValue(info));
    });
    connect(m_scanner, &BleScanner::errorOccurred, this, [this](const QString &errStr) {
        QMessageBox::warning(this, tr("Error Occurred"), errStr);
    });

    ui->progressBarOpening->hide();
    ui->progressBarScanning->hide();
    ui->labelUnsupported->hide();
    ui->labelWriteMode->hide();
    ui->comboBoxWriteMode->hide();
    ui->pushButtonRead->hide();
    ui->pushButtonNotify->hide();
}

BleCentralUi::~BleCentralUi()
{
    delete ui;
}

QVariantMap BleCentralUi::save() const
{
    QVariantMap map;
    map["deviceInfo"] = ui->comboBoxDevices->currentData();
    map["service"] = ui->comboBoxServices->currentData();
    map["writeMode"] = ui->comboBoxWriteMode->currentData();
    map["characteristic"] = ui->comboBoxCharacteristics->currentData();
    return map;
}

void BleCentralUi::load(const QVariantMap &parameters)
{
    Q_UNUSED(parameters);
}

void BleCentralUi::setupDevice(Device *device)
{
    BleCentral *central = qobject_cast<BleCentral *>(device);
    if (!central) {
        resetVisible();

        ui->comboBoxServices->clear();
        ui->comboBoxCharacteristics->clear();
        ui->pushButtonScan->setEnabled(true);
        return;
    }

    ui->pushButtonScan->setEnabled(false);
    ui->comboBoxServices->clear();
    ui->comboBoxCharacteristics->clear();
    ui->progressBarOpening->show();
    connect(central, &BleCentral::discoveryFinished, this, [this]() {
        ui->progressBarOpening->hide();
    });
    connect(central, &BleCentral::serviceDiscovered, this, [this](QLowEnergyService *service) {
        setupLowEnergyService(service);
        ui->comboBoxServices->addItem(service->serviceName(), QVariant::fromValue(service));
    });
}

void BleCentralUi::setUiEnabled(bool enabled)
{
    ui->comboBoxDevices->setEnabled(enabled);
}

void BleCentralUi::updateNotifyText()
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

void BleCentralUi::resetVisible()
{
    ui->labelWriteMode->hide();
    ui->comboBoxWriteMode->hide();
    ui->pushButtonRead->hide();
    ui->pushButtonNotify->hide();
    ui->labelUnsupported->hide();
    ui->progressBarOpening->hide();
}

void BleCentralUi::setNotifyText(const QByteArray &value)
{
    if (value == notifyValue()) {
        ui->pushButtonNotify->setText(tr("Disnotify"));
    } else {
        ui->pushButtonNotify->setText(tr("Notify"));
    }
}

void BleCentralUi::setupWriteMode(bool enableWithResponse, bool writeWithoutResponse)
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

void BleCentralUi::setupLowEnergyService(QLowEnergyService *service)
{
    if (!service) {
        qWarning() << "Service is null";
        return;
    }

    connect(service,
            &QLowEnergyService::descriptorWritten,
            this,
            &BleCentralUi::onDescriptorWritten,
            Qt::QueuedConnection);
    connect(service,
            &QLowEnergyService::descriptorRead,
            this,
            &BleCentralUi::onDescriptorRead,
            Qt::QueuedConnection);
}

void BleCentralUi::onScanButtonClicked()
{
    ui->pushButtonScan->setEnabled(false);

    if (m_scanner->isRunning()) {
        m_scanner->exit();
    } else {
        ui->comboBoxDevices->clear();
        m_scanner->start();
    }
}

void BleCentralUi::onServiceIndexChanged(int index)
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
    for (const QLowEnergyCharacteristic &characteristic : service->characteristics()) {
        QVariant var = QVariant::fromValue(characteristic);
        QString name = characteristic.name();
        name = name.trimmed();
        if (name.isEmpty()) {
            name = tr("(No name)");
        }
        ui->comboBoxCharacteristics->addItem(name, var);
    }
}

void BleCentralUi::onCharacteristicIndexChanged(int index)
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

void BleCentralUi::onDescriptorWritten(const QLowEnergyDescriptor &info, const QByteArray &value)
{
    qInfo() << "Descriptor written" << info.name() << info.uuid();
    setNotifyText(value);
}

void BleCentralUi::onDescriptorRead(const QLowEnergyDescriptor &info, const QByteArray &value)
{
    qInfo() << "Descriptor read" << info.name() << info.uuid();
    setNotifyText(value);
}

void BleCentralUi::onNotifyButtonClicked()
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

void BleCentralUi::onReadButtonClicked()
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

QByteArray BleCentralUi::notifyValue() const
{
    static const QByteArray notifyValue = QByteArray::fromHex("0100");
    return notifyValue;
}

QByteArray BleCentralUi::disnotifyValue() const
{
    static const QByteArray disnotifyValue = QByteArray::fromHex("0000");
    return disnotifyValue;
}
