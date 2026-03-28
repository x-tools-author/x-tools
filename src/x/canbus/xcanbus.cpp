/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
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

#include "canbuscommon.h"
#include "canbusdevice.h"
#include "datamodel.h"
#include "framelistview.h"

namespace xCanBus {

struct xCanBusParameterKeys
{
    const QString rightWidth{"rightWidth"};
    const QString tabIndex{"tabIndex"};

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
    ui->splitter->setSizes({width() - m_rightWidth, m_rightWidth});
    ui->comboBoxPlugins->addItems(QCanBus::instance()->plugins());
    ui->comboBoxBitrate->setEditable(true);
    ui->comboBoxDataBitrate->setEditable(true);
    ui->pushButtonDisconnect->setEnabled(false);
    ui->stackedWidget->hide();
    connect(ui->pushButtonConnect, &QPushButton::clicked, this, &xCanBus::onConnectBtnClicked);
    connect(ui->pushButtonDisconnect, &QPushButton::clicked, this, &xCanBus::onDisconnectBtnClicked);
    connect(ui->comboBoxPlugins, &QComboBox::currentTextChanged, this, &xCanBus::onPluginChanged);
    connect(ui->pushButtonSend, &QPushButton::clicked, this, &xCanBus::onSendBtnClicked);
    connect(ui->comboBoxInputFormat, xComboBoxActivated, this, &xCanBus::updateInputValidator);
    connect(ui->comboBoxTimedSending, xComboBoxActivated, this, &xCanBus::onTimedSendingChanged);
    connect(ui->lineEditPayload, &QLineEdit::returnPressed, this, &xCanBus::onSendBtnClicked);
    connect(ui->splitter, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        this->m_rightWidth = ui->splitter->sizes().first();
    });

    struct FrameErrorItem
    {
        int type;
        QString name;
    };
    // clang-format off
    QList<FrameErrorItem> errorItems = {
        {QCanBusFrame::NoError, tr("No error has occurred")},
        {QCanBusFrame::TransmissionTimeoutError, tr("The transmission has timed out")},
        {QCanBusFrame::LostArbitrationError, tr("The frame could not be sent due to lost arbitration on the bus")},
        {QCanBusFrame::ControllerError, tr("The controller encountered an error")},
        {QCanBusFrame::ProtocolViolationError, tr("A protocol violation has occurred")},
        {QCanBusFrame::TransceiverError, tr("A transceiver error occurred")},
        {QCanBusFrame::MissingAcknowledgmentError, tr("The transmission received no acknowledgment")},
        {QCanBusFrame::BusOffError, tr("The CAN bus is offline")},
        {QCanBusFrame::BusError, tr("A CAN bus error occurred")},
        {QCanBusFrame::ControllerRestartError, tr("The controller restarted")},
        {QCanBusFrame::UnknownError, tr("An unknown error has occurred")},
        {QCanBusFrame::AnyError, tr("Matches every other error type")},
    };
    // clang-format on
    m_menu = new xTools::KeepOpenedMenu(this);
    for (int i = 0; i < errorItems.count(); ++i) {
        FrameErrorItem item = errorItems.at(i);
        QAction* a = m_menu->addAction(item.name, this, &xCanBus::updateErrorFilterBtn);
        a->setCheckable(true);
        a->setData(QVariant(item.type));
    }
    ui->pushButtonErrorFilter->setMenu(m_menu);
    updateErrorFilterBtn();
    updateInputValidator();

    setupBitRates(ui->comboBoxBitrate, false);
    setupBitRates(ui->comboBoxDataBitrate, true);
    setupOptions(ui->comboBoxReceiveOwn, true);
    setupOptions(ui->comboBoxLoopback, true);
    setupOptions(ui->comboBoxCanFd, false);
    setupTimedSending(ui->comboBoxTimedSending);
    xSetupFrameTypes(ui->comboBoxFrameType);
    xSetupTextFormat(ui->comboBoxInputFormat);

    m_device = new CanBusDevice(this);
    connect(m_device, &CanBusDevice::opened, this, &xCanBus::onDeviceOpened);
    connect(m_device, &CanBusDevice::finished, this, &xCanBus::onDeviceClosed);
    connect(m_device, &CanBusDevice::frameRx, this, &xCanBus::onFrameRx);
    connect(m_device, &CanBusDevice::frameTx, this, &xCanBus::onFrameTx);

    m_timedSender = new QTimer(this);
    connect(m_timedSender, &QTimer::timeout, this, &xCanBus::onTimedSenderTimeout);
    onTimedSendingChanged();
#if 1
    // Add panels
    FrameListView* frameListPanel = new FrameListView(this);
    addPanel("frameList", tr("Frame List"), frameListPanel);
#endif
}

xCanBus::~xCanBus()
{
    delete ui;
}

QJsonObject xCanBus::save()
{
    QJsonObject obj;
    xCanBusParameterKeys keys;
    obj.insert(keys.rightWidth, ui->stackedWidget->width());
    int index = -1;
    for (int i = 0; i < m_panels.count(); i++) {
        const auto& item = m_panels.at(i);
        if (ui->stackedWidget->currentWidget() == item.panel) {
            index = i;
            break;
        }
    }
    obj.insert(keys.tabIndex, index);

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
    obj.insert(keys.frameId, ui->spinBoxFrameId->value());
    obj.insert(keys.extenedFrame, ui->checkBoxExtendedFormat->isChecked());
    obj.insert(keys.bitrateSwitch, ui->checkBoxBitrateSwitch->isChecked());
    obj.insert(keys.flexibleDataRate, ui->checkBoxFlexibleDataRate->isChecked());
    obj.insert(keys.payload, ui->lineEditPayload->text().trimmed());

    obj.insert(keys.dataView, ui->widgetDataView->save());

    for (const auto& item : m_panels) {
        QJsonObject panelObj = item.panel->save();
        obj.insert(item.key, panelObj);
    }

    return obj;
}

void xCanBus::load(const QJsonObject& obj)
{
    xCanBusParameterKeys keys;
    int rightWidth = obj.value(keys.rightWidth).toInt(400);
    ui->splitter->setSizes({width() - rightWidth, rightWidth});
    int tabIndex = obj.value(keys.tabIndex).toInt(-1);
    if (tabIndex >= 0 && tabIndex < m_panels.count()) {
        ui->stackedWidget->setCurrentWidget(m_panels.at(tabIndex).panel);
        ui->stackedWidget->show();
        m_panels.at(tabIndex).btn->setChecked(true);
    }

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
    int frameIdentifier = obj.value(keys.frameId).toInt();
    ui->spinBoxFrameId->setValue(frameIdentifier);
    ui->checkBoxExtendedFormat->setChecked(obj.value(keys.extenedFrame).toBool());
    ui->checkBoxFlexibleDataRate->setChecked(obj.value(keys.flexibleDataRate).toBool());
    ui->checkBoxBitrateSwitch->setChecked(obj.value(keys.bitrateSwitch).toBool());
    updateInputValidator();
    ui->lineEditPayload->setText(obj.value(keys.payload).toString());

    ui->widgetDataView->load(obj.value(keys.dataView).toObject());

    for (const auto& item : m_panels) {
        QJsonObject panelObj = obj.value(item.key).toObject();
        item.panel->load(panelObj);
    }
}

bool xCanBus::event(QEvent* event)
{
    if (event->type() == QEvent::Resize) {
        ui->splitter->setSizes({m_rightWidth, width() - m_rightWidth});
    }

    return QWidget::event(event);
}

void xCanBus::addPanel(const QString& key, const QString& title, CanBusPanel* panel)
{
    QToolButton* btn = new QToolButton(this);
    btn->setText(title);
    btn->setCheckable(true);
    ui->stackedWidget->addWidget(panel);
    ui->horizontalLayoutTab->addWidget(btn);

    connect(btn, &QToolButton::clicked, this, [this, key](bool checked) {
        for (const auto& item : m_panels) {
            if (item.key == key) {
                ui->stackedWidget->setCurrentWidget(item.panel);
            } else {
                item.btn->setChecked(false);
            }
        }

        this->ui->stackedWidget->setVisible(checked);
    });

    connect(panel, &CanBusPanel::outputFrame, this, [this](const QCanBusFrame& frame) {
        if (m_device && m_device->isRunning()) {
            m_device->writeFrame(frame);
        }
    });

    m_panels.append({key, panel, btn});
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
        int errorFilterInt = this->errorFilter().toInt();
#else
        int errorFilterInt = this->errorFilter();
#endif
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
    QString payloadStr = ui->lineEditPayload->text().trimmed();
    if (payloadStr.isEmpty()) {
        return;
    }

    sendFrame();
}

void xCanBus::onFrameRx(const QCanBusFrame& frame)
{
    DataModel* model = ui->widgetDataView->model();
    model->addFrame(frame, true);
}

void xCanBus::onFrameTx(const QCanBusFrame& frame)
{
    DataModel* model = ui->widgetDataView->model();
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
#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
    int errorFilterInt = errorFilter().toInt();
#else
    int errorFilterInt = errorFilter();
#endif
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
    xSetupTextFormatValidator(ui->lineEditPayload, format);
}

void xCanBus::sendFrame()
{
    if (!m_device || (m_device && !m_device->isRunning())) {
        return;
    }

    const QString txt = ui->lineEditPayload->text();
    const QByteArray payload = xString2bytes(txt, ui->comboBoxInputFormat->currentData().toInt());
    const uint frameId = ui->spinBoxFrameId->value();
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
