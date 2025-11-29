/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttclientui.h"
#include "ui_mqttclientui.h"

#include <QMessageBox>

#include "utilities/iconengine.h"
#include "utilities/x.h"

#include "mqttclient.h"
#include "mqttcommon.h"

namespace xMQTT {

struct MqttClientUiParameterKeys
{
    const QString serverAddress{"serverAddress"};
    const QString serverPort{"serverPort"};
    const QString qos{"qos"};
    const QString versions{"versions"};
    const QString keepAlive{"keepAlive"};
    const QString leftWidth{"leftWidth"};
    const QString publishTopic{"publishTopic"};
    const QString publicMessage{"message"};
    const QString subscribeTopic{"subscribeTopic"};
    const QString wrapText{"wrapText"};
    const QString leftTabIndex{"leftTabIndex"};
};

MqttClientUi::MqttClientUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttClientUi)
{
    ui->setupUi(this);
#if 0
    xSetNoneBorder(ui->textEditPublish);
#endif
    xSetNoneBorder(ui->listViewSubscribe);
    xSetNoneBorder(ui->textBrowserLog);
    setupSocketAddress(ui->comboBoxServerAddress);
    setupPortSpinBox(ui->spinBoxServerPort);
    setupQosComboBox(ui->comboBoxQos);
    setupVersionComboBox(ui->comboBoxVersions);
    setupMessageFormatComboBox(ui->comboBoxTextFormat);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes(QList<int>({m_leftWidth, width() - m_leftWidth}));
    ui->textBrowserLog->setWordWrapMode(QTextOption::NoWrap);
    ui->toolButtonPublish->setIcon(xIcon(":/res/icons/upload.svg"));
    ui->toolButtonSubscribe->setIcon(xIcon(":/res/icons/download.svg"));
    ui->toolButtonWrap->setIcon(xIcon(":/res/icons/wrap_text.svg"));
    ui->toolButtonTimer->setIcon(xIcon(":/res/icons/timer.svg"));
    ui->toolButtonTimer->setCheckable(true);
    ui->toolButtonWrap->setCheckable(true);
    ui->toolButtonWrap->setChecked(true);
    ui->textBrowserLog->document()->setMaximumBlockCount(1024);
    // clang-format off
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MqttClientUi::onOpenButtonClicked);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &MqttClientUi::onCloseButtonClicked);
    connect(ui->toolButtonPublish, &QToolButton::clicked, this, &MqttClientUi::onPublishButtonClicked);
    connect(ui->toolButtonSubscribe, &QToolButton::clicked, this, &MqttClientUi::onSubscribeButtonClicked);
    // clang-format on
    connect(ui->splitter, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        Q_UNUSED(index);
        Q_UNUSED(pos);
        this->m_leftWidth = ui->widgetClient->width();
    });
    connect(ui->toolButtonWrap, &QToolButton::clicked, this, [=](bool checked) {
        ui->textEditPublish->setLineWrapMode(checked ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    });

    m_client = new MqttClient(this);
    connect(m_client, &MqttClient::logMessage, this, &MqttClientUi::onLogMessageReceived);
}

MqttClientUi::~MqttClientUi()
{
    delete ui;
}

QJsonObject MqttClientUi::save()
{
    MqttClientUiParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.serverAddress, ui->comboBoxServerAddress->currentText());
    obj.insert(keys.serverPort, ui->spinBoxServerPort->value());
    obj.insert(keys.qos, ui->comboBoxQos->currentIndex());
    obj.insert(keys.versions, ui->comboBoxVersions->currentIndex());
    obj.insert(keys.keepAlive, ui->spinBoxKeepAlive->value());
    obj.insert(keys.leftWidth, ui->widgetClient->width());
    // Publish panel
    obj.insert(keys.publishTopic, ui->lineEditPublish->text());
    obj.insert(keys.publicMessage, ui->textEditPublish->toPlainText());
    obj.insert(keys.wrapText, ui->toolButtonWrap->isChecked());
    obj.insert(keys.leftTabIndex, ui->tabWidgetLeft->currentIndex());
    // Subscribe panel
    obj.insert(keys.subscribeTopic, ui->lineEditSubscribe->text());
    return obj;
}

void MqttClientUi::load(const QJsonObject &obj)
{
    MqttClientUiParameterKeys keys;
    int index = ui->comboBoxServerAddress->findText(obj.value(keys.serverAddress).toString());
    ui->comboBoxServerAddress->setCurrentIndex(index >= 0 ? index : 0);
    ui->spinBoxServerPort->setValue(obj.value(keys.serverPort).toInt(1883));
    index = obj.value(keys.qos).toInt(static_cast<int>(QoS::AtMostOnce));
    ui->comboBoxQos->setCurrentIndex(index >= 0 && index < ui->comboBoxQos->count() ? index : 0);
    int leftWidth = obj.value(keys.leftWidth).toInt(m_leftWidth);
    ui->splitter->setSizes({leftWidth, width() - leftWidth});
    int version = obj.value(keys.versions).toInt(static_cast<int>(4));
    index = ui->comboBoxVersions->findData(version);
    ui->comboBoxVersions->setCurrentIndex(index == -1 ? 0 : index);
    ui->spinBoxKeepAlive->setValue(obj.value(keys.keepAlive).toInt(5));

    // Publish panel
    ui->lineEditPublish->setText(obj.value(keys.publishTopic).toString());
    ui->textEditPublish->setPlainText(obj.value(keys.publicMessage).toString());
    bool wrapText = obj.value(keys.wrapText).toBool(true);
    ui->toolButtonWrap->setChecked(wrapText);
    ui->textEditPublish->setLineWrapMode(wrapText ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    index = obj.value(keys.leftTabIndex).toInt(0);
    ui->tabWidgetLeft->setCurrentIndex(index);

    // Subscribe panel
    ui->lineEditSubscribe->setText(obj.value(keys.subscribeTopic).toString());
}

bool MqttClientUi::event(QEvent *event)
{
    if (ui && event->type() == QEvent::Resize) {
        ui->splitter->setSizes({m_leftWidth, width() - m_leftWidth});
    }

    return QWidget::event(event);
}

void MqttClientUi::onOpenButtonClicked()
{
    if (!m_client) {
        return;
    }

    QString host = ui->comboBoxServerAddress->currentText();
    quint16 port = static_cast<quint16>(ui->spinBoxServerPort->value());
    int qos = ui->comboBoxQos->currentData().toInt();
    int version = ui->comboBoxVersions->currentData().toInt();
    m_client->startClient(host, port, qos, version);
}

void MqttClientUi::onCloseButtonClicked()
{
    if (m_client) {
        m_client->stopClient();
    }
}

void MqttClientUi::onPublishButtonClicked()
{
    if (!m_client || !m_client->isOpened()) {
        showNotOpenedWarning();
        return;
    }

    if (ui->lineEditPublish->text().trimmed().isEmpty()) {
        showEmptyTopicWarning();
        ui->lineEditPublish->setFocus();
        return;
    }

    QString topic = ui->lineEditPublish->text().trimmed();
    QString txt = ui->textEditPublish->toPlainText();
    int data = ui->comboBoxTextFormat->currentData().toInt();
    QByteArray msg = cookedMessage(txt, static_cast<MessageFormat>(data));
    m_client->publish(topic, msg);
}

void MqttClientUi::onSubscribeButtonClicked()
{
    if (!m_client || !m_client->isOpened()) {
        showNotOpenedWarning();
        return;
    }

    if (ui->lineEditSubscribe->text().trimmed().isEmpty()) {
        showEmptyTopicWarning();
        ui->lineEditSubscribe->setFocus();
        return;
    }

    m_client->subscribe(ui->lineEditSubscribe->text().trimmed());
}

void MqttClientUi::onLogMessageReceived(const QString &msg, bool isError)
{
    ui->textBrowserLog->append(msg);
}

void MqttClientUi::showNotOpenedWarning()
{
    QMessageBox::warning(this, tr("Warning"), tr("The MQTT client is not opened."));
}

void MqttClientUi::showEmptyTopicWarning()
{
    QMessageBox::warning(this, tr("Warning"), tr("The topic is empty."));
}

} // namespace xMQTT