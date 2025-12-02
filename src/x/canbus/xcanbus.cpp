/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xcanbus.h"
#include "ui_xcanbus.h"

#include <QCanBus>
#include <QCanBusDeviceInfo>
#include <QJsonArray>
#include <QMessageBox>
#include <QMetaEnum>

#include "common/xtools.h"
#include "utilities/compatibility.h"
#include "utilities/iconengine.h"

#include "canbusdevice.h"
#include "datamodel.h"

namespace xCanBus {

struct xCanBusParameterKeys
{
    const QString leftWidth{"leftWidth"};

    const QString plugin{"plugin"};
    const QString interface{"interface"};
    const QString errorFilter{"errorFilter"};
    const QString bitrate{"bitrate"};
    const QString dataBitrate{"dataBitrate"};
    const QString loopback{"loopback"};
    const QString receiveOwn{"receiveOwn"};
    const QString canFd{"canFd"};

    const QString inputFormat{"inputFormat"};
    const QString frameType{"frameType"};
    const QString frameId{"frameId"};
    const QString extenedFrame{"extenedFrame"};
    const QString flexibleDataRate{"flexibleDataRate"};
    const QString bitrateSwitch{"bitrateSwitch"};
    const QString payload{"payload"};

    const QString dataView{"dataView"};
};

xCanBus::xCanBus(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xCanBus)
{
    ui->setupUi(this);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes({m_leftWidth, width() - m_leftWidth});
    ui->comboBoxPlugins->addItems(QCanBus::instance()->plugins());
    ui->comboBoxBitrate->setEditable(true);
    ui->comboBoxDataBitrate->setEditable(true);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->toolButtonSend->setIcon(xIcon(":res/icons/send.svg"));
    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &xCanBus::onConnectBtnClicked);
    connect(ui->pushButtonDisconnect, &QPushButton::clicked, this, &xCanBus::onDisconnectBtnClicked);
    connect(ui->comboBoxPlugins, &QComboBox::currentTextChanged, this, &xCanBus::onPluginChanged);
    connect(ui->toolButtonSend, &QToolButton::clicked, this, &xCanBus::onSendBtnClicked);
    connect(ui->comboBoxInputFormat, xComboBoxActivated, this, &xCanBus::updateInputValidator);
    connect(ui->comboBoxTimedSending, xComboBoxActivated, this, &xCanBus::onTimedSendingChanged);
    connect(ui->lineEditPayload, &QLineEdit::returnPressed, this, &xCanBus::onSendBtnClicked);
    connect(ui->splitter, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        this->m_leftWidth = ui->splitter->sizes().first();
    });

    m_menu = new xTools::KeepOpenedMenu(this);
    QMetaEnum metaEnum = QMetaEnum::fromType<QCanBusFrame::FrameError>();
    for (int i = 0; i < metaEnum.keyCount(); ++i) {
        QString key = QString::fromUtf8(metaEnum.key(i));
        QAction* a = m_menu->addAction(key, this, &xCanBus::updateErrorFilterBtn);
        a->setCheckable(true);
        a->setData(QVariant(metaEnum.value(i)));
    }
    ui->pushButtonErrorFilter->setMenu(m_menu);
    updateErrorFilterBtn();
    updateInputValidator();

    setupBitRates(ui->comboBoxBitrate, false);
    setupBitRates(ui->comboBoxDataBitrate, true);
    setupOptions(ui->comboBoxReceiveOwn, true);
    setupOptions(ui->comboBoxLoopback, true);
    setupOptions(ui->comboBoxCanFd, false);
    setupFrameTypes(ui->comboBoxFrameType);
    setupTimedSending(ui->comboBoxTimedSending);
    setupTextFormat(ui->comboBoxInputFormat);
    setupTextFormatValidator(ui->lineEditFrameId, static_cast<int>(TextFormat::HexWithoutSpace), 8);

    m_device = new CanBusDevice(this);
    connect(m_device, &CanBusDevice::opened, this, &xCanBus::onDeviceOpened);
    connect(m_device, &CanBusDevice::finished, this, &xCanBus::onDeviceClosed);
    connect(m_device, &CanBusDevice::frameRx, this, &xCanBus::onFrameRx);
    connect(m_device, &CanBusDevice::frameTx, this, &xCanBus::onFrameTx);

    m_timedSender = new QTimer(this);
    connect(m_timedSender, &QTimer::timeout, this, &xCanBus::onTimedSenderTimeout);
    onTimedSendingChanged();
}

xCanBus::~xCanBus()
{
    delete ui;
}

QJsonObject xCanBus::save()
{
    QJsonObject obj;
    xCanBusParameterKeys keys;
    obj.insert(keys.leftWidth, ui->splitter->sizes().first());

    obj.insert(keys.plugin, ui->comboBoxPlugins->currentText());
    obj.insert(keys.interface, ui->comboBoxName->currentText());
    QJsonArray errorFilter;
    const QList<QAction*> actions = m_menu->actions();
    for (int i = 0; i < actions.count(); i++) {
        QAction* action = actions.at(i);
        if (action->isChecked()) {
            errorFilter.append(action->data().toInt());
        }
    }
    obj.insert(keys.errorFilter, errorFilter);
    obj.insert(keys.bitrate, ui->comboBoxBitrate->currentText());
    obj.insert(keys.dataBitrate, ui->comboBoxDataBitrate->currentText());
    obj.insert(keys.loopback, ui->comboBoxLoopback->currentText());
    obj.insert(keys.receiveOwn, ui->comboBoxReceiveOwn->currentText());
    obj.insert(keys.canFd, ui->comboBoxCanFd->currentText());

    obj.insert(keys.inputFormat, ui->comboBoxInputFormat->currentData().toInt());
    obj.insert(keys.frameType, ui->comboBoxFrameType->currentData().toInt());
    obj.insert(keys.frameId, ui->lineEditFrameId->text().trimmed());
    obj.insert(keys.extenedFrame, ui->checkBoxExtendedFormat->isChecked());
    obj.insert(keys.bitrateSwitch, ui->checkBoxBitrateSwitch->isChecked());
    obj.insert(keys.flexibleDataRate, ui->checkBoxFlexibleDataRate->isChecked());
    obj.insert(keys.payload, ui->lineEditPayload->text().trimmed());

    obj.insert(keys.dataView, ui->tabWidgetData->save());

    return obj;
}

void xCanBus::load(const QJsonObject& obj)
{
    xCanBusParameterKeys keys;
    int leftWidth = obj.value(keys.leftWidth).toInt(168);
    ui->splitter->setSizes({leftWidth, width() - leftWidth});

    ui->comboBoxPlugins->setCurrentText(obj.value(keys.plugin).toString());
    ui->comboBoxName->setCurrentText(obj.value(keys.interface).toString());
    QJsonArray errorFilter = obj.value(keys.errorFilter).toArray();
    for (int i = 0; i < errorFilter.count(); i++) {
        QVariant v = errorFilter.at(i).toVariant();
        QList<QAction*> actions = m_menu->actions();
        for (int j = 0; j < actions.count(); j++) {
            QAction* action = actions.at(j);
            if (v == action->data()) {
                action->setChecked(true);
            }
        }
    }
    ui->comboBoxBitrate->setCurrentText(obj.value(keys.bitrate).toString("50000"));
    ui->comboBoxDataBitrate->setCurrentText(obj.value(keys.dataBitrate).toString("50000"));
    ui->comboBoxLoopback->setCurrentText(obj.value(keys.loopback).toString());
    ui->comboBoxReceiveOwn->setCurrentText(obj.value(keys.receiveOwn).toString());
    ui->comboBoxCanFd->setCurrentText(obj.value(keys.canFd).toString());

    int inputFormat = obj.value(keys.inputFormat).toInt();
    int index = ui->comboBoxInputFormat->findData(inputFormat);
    ui->comboBoxInputFormat->setCurrentIndex(index < 0 ? 4 : index);
    int frameType = obj.value(keys.frameType).toInt();
    index = ui->comboBoxFrameType->findData(frameType);
    ui->comboBoxFrameType->setCurrentIndex(index < 0 ? 0 : index);
    ui->lineEditFrameId->setText(obj.value(keys.frameId).toString());
    ui->checkBoxExtendedFormat->setChecked(obj.value(keys.extenedFrame).toBool());
    ui->checkBoxFlexibleDataRate->setChecked(obj.value(keys.flexibleDataRate).toBool());
    ui->checkBoxBitrateSwitch->setChecked(obj.value(keys.bitrateSwitch).toBool());
    updateInputValidator();
    ui->lineEditPayload->setText(obj.value(keys.payload).toString());

    ui->tabWidgetData->load(obj.value(keys.dataView).toObject());
}

bool xCanBus::event(QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        ui->splitter->setSizes({m_leftWidth, width() - m_leftWidth});
    }

    return QWidget::event(event);
}

void xCanBus::onDisconnectBtnClicked()
{
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(false);
    m_device->stopDevice();
}

void xCanBus::onConnectBtnClicked()
{
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(false);

    CanBusDeviceParameters params;
    params.plugin = ui->comboBoxPlugins->currentText();
    params.interfaceName = ui->comboBoxName->currentText();

    QList<QPair<QCanBusDevice::ConfigurationKey, QVariant>> tmp;
    int bitRate = ui->comboBoxBitrate->currentText().toInt();
    int dataBitRate = ui->comboBoxDataBitrate->currentText().toInt();
    tmp.append(qMakePair(QCanBusDevice::BitRateKey, bitRate));
    tmp.append(qMakePair(QCanBusDevice::DataBitRateKey, dataBitRate));
    tmp.append(qMakePair(QCanBusDevice::CanFdKey, ui->comboBoxCanFd->currentData()));
    if (ui->comboBoxLoopback->currentData().isValid()) {
        tmp.append(qMakePair(QCanBusDevice::LoopbackKey, ui->comboBoxLoopback->currentData()));
    }

    if (ui->comboBoxReceiveOwn->currentData().isValid()) {
        tmp.append(qMakePair(QCanBusDevice::ReceiveOwnKey, ui->comboBoxReceiveOwn->currentData()));
    }

    if (hasErrorFilter()) {
        int errorFilterInt = this->errorFilter().toInt();
        tmp.append(qMakePair(QCanBusDevice::ErrorFilterKey, QVariant(errorFilterInt)));
    }

    params.params = tmp;
    m_device->startDevice(params);
}

void xCanBus::onDeviceOpened()
{
    ui->pushButtonConnect->setEnabled(false);
    ui->pushButtonDisconnect->setEnabled(true);
    updateUiState(true);
}

void xCanBus::onDeviceClosed()
{
    ui->pushButtonConnect->setEnabled(true);
    ui->pushButtonDisconnect->setEnabled(false);
    updateUiState(false);
}

void xCanBus::onPluginChanged(const QString& pluginName)
{
    ui->comboBoxName->clear();
    QList<QCanBusDeviceInfo> devices = QCanBus::instance()->availableDevices(pluginName);
    for (auto& device : devices) {
        ui->comboBoxName->addItem(device.name());
    }
}

void xCanBus::onSendBtnClicked()
{
    if (!m_device || (m_device && !m_device->isRunning())) {
        QMessageBox::warning(this, tr("Warning"), tr("The CAN bus device is not connected."));
        return;
    }

    bool ok = false;
    QString idStr = ui->lineEditFrameId->text().trimmed();
    uint id = idStr.toUInt(&ok, 16);
    Q_UNUSED(id);
    if (!ok) {
        QMessageBox::warning(this, tr("Warning"), tr("Invalid frame ID."));
        ui->lineEditFrameId->setFocus();
        return;
    }

    QString payloadStr = ui->lineEditPayload->text().trimmed();
    if (payloadStr.isEmpty()) {
        QMessageBox::warning(this, tr("Warning"), tr("Payload is empty."));
        ui->lineEditPayload->setFocus();
        return;
    }

    sendFrame();
}

void xCanBus::onTimedSendingChanged()
{
    if (!m_timedSender) {
        return;
    }

    int interval = ui->comboBoxTimedSending->currentData().toInt();
    if (interval <= 0) {
        m_timedSender->stop();
    } else {
        m_timedSender->start(interval);
    }
}

void xCanBus::onTimedSenderTimeout()
{
    QString idStr = ui->lineEditFrameId->text().trimmed();
    if (idStr.isEmpty()) {
        return;
    }

    bool ok = false;
    uint id = idStr.toUInt(&ok, 16);
    Q_UNUSED(id);
    if (!ok) {
        return;
    }

    QString payloadStr = ui->lineEditPayload->text().trimmed();
    if (payloadStr.isEmpty()) {
        return;
    }

    sendFrame();
}

void xCanBus::onFrameRx(const QCanBusFrame& frame)
{
    DataModel* model = ui->tabWidgetData->model();
    model->addFrame(frame, true);
}

void xCanBus::onFrameTx(const QCanBusFrame& frame)
{
    DataModel* model = ui->tabWidgetData->model();
    model->addFrame(frame, false);
}

void xCanBus::setupOptions(QComboBox* cb, bool usingUnspecified)
{
    if (cb) {
        cb->clear();
        if (usingUnspecified) {
            cb->addItem(tr("unspecified"), QVariant());
        }
        cb->addItem(QString("false"), QVariant(false));
        cb->addItem(QString("true"), QVariant(true));
    }
}

void xCanBus::setupBitRates(QComboBox* cb, bool isFlexibleDataRateEnable)
{
    if (!cb) {
        return;
    }

    const QVector<int> rates = {10000, 20000, 50000, 100000, 125000, 250000, 500000, 800000, 1000000};
    const QVector<int> dataRates = {2000000, 4000000, 8000000};
    cb->clear();

    for (int rate : rates) {
        cb->addItem(QString::number(rate), rate);
    }

    if (isFlexibleDataRateEnable) {
        for (int rate : dataRates) {
            cb->addItem(QString::number(rate), rate);
        }
    }
}

void xCanBus::setupFrameTypes(QComboBox* cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    cb->addItem(tr("Data Frame"), QVariant(QCanBusFrame::DataFrame));
    cb->addItem(tr("Remote Request Frame"), QVariant(QCanBusFrame::RemoteRequestFrame));
    cb->addItem(tr("Error Frame"), QVariant(QCanBusFrame::ErrorFrame));
}

void xCanBus::setupTimedSending(QComboBox* cb)
{
    if (!cb) {
        return;
    }

    cb->clear();
    cb->addItem(tr("Disabled"), QVariant(-1));
    for (int i = 100; i <= 1000; i += 100) {
        cb->addItem(QString::number(i) + " ms", QVariant(i));
    }
    for (int i = 2000; i <= 10000; i += 1000) {
        cb->addItem(QString::number(i) + " ms", QVariant(i));
    }
}

QCanBusFrame::FrameErrors xCanBus::errorFilter() const
{
    QCanBusFrame::FrameErrors filter = QCanBusFrame::NoError;
    QList<QAction*> actions = m_menu->actions();
    for (QAction* a : actions) {
        if (a->isChecked()) {
            filter |= static_cast<QCanBusFrame::FrameError>(a->data().toInt());
        }
    }

    return filter;
}

bool xCanBus::hasErrorFilter() const
{
    QList<QAction*> actions = m_menu->actions();
    for (QAction* a : actions) {
        if (a->isChecked()) {
            return true;
        }
    }
    return false;
}

void xCanBus::updateErrorFilterBtn()
{
    if (!hasErrorFilter()) {
        ui->pushButtonErrorFilter->setText(tr("Unspecified"));
        return;
    }

    int errorFilterInt = errorFilter().toInt();
    QString txt = QString::number(errorFilterInt, 16).toUpper();
    while (txt.length() < 8) {
        txt.prepend('0');
    }
    txt.prepend("0x");
    ui->pushButtonErrorFilter->setText(txt);
}

void xCanBus::updateInputValidator()
{
    if (!ui->lineEditPayload) {
        return;
    }

    int format = static_cast<int>(TextFormat::HexWithoutSpace);
    setupTextFormatValidator(ui->lineEditPayload, format);
}

void xCanBus::sendFrame()
{
    if (!m_device || (m_device && !m_device->isRunning())) {
        return;
    }

    const QString txt = ui->lineEditPayload->text();
    const QByteArray payload = string2bytes(txt, ui->comboBoxInputFormat->currentData().toInt());
    const uint frameId = static_cast<uint>(ui->lineEditFrameId->text().toUInt(nullptr, 16));
    QCanBusFrame frame = QCanBusFrame(frameId, payload);
    frame.setExtendedFrameFormat(ui->checkBoxExtendedFormat->isChecked());
    frame.setFlexibleDataRateFormat(ui->checkBoxFlexibleDataRate->isChecked());
    frame.setBitrateSwitch(ui->checkBoxBitrateSwitch->isChecked());
    const int frameType = ui->comboBoxFrameType->currentData().toInt();
    frame.setFrameType(static_cast<QCanBusFrame::FrameType>(frameType));
    m_device->writeFrame(frame);
}

void xCanBus::updateUiState(bool isConnected)
{
    ui->comboBoxPlugins->setEnabled(!isConnected);
    ui->comboBoxName->setEnabled(!isConnected);
    ui->comboBoxBitrate->setEnabled(!isConnected);
    ui->comboBoxDataBitrate->setEnabled(!isConnected);
    ui->comboBoxLoopback->setEnabled(!isConnected);
    ui->comboBoxReceiveOwn->setEnabled(!isConnected);
    ui->comboBoxCanFd->setEnabled(!isConnected);
}

} // namespace xCanBus