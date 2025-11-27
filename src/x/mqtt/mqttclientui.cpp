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
};

MqttClientUi::MqttClientUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttClientUi)
{
    ui->setupUi(this);
    setupSocketAddress(ui->comboBoxServerAddress);
    setupPortSpinBox(ui->spinBoxServerPort);
    setupQosComboBox(ui->comboBoxQos);
    setupVersionComboBox(ui->comboBoxVersions);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes(QList<int>({m_leftWidth, width() - m_leftWidth}));
    ui->textBrowser->setWordWrapMode(QTextOption::NoWrap);
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MqttClientUi::onOpenButtonClicked);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &MqttClientUi::onCloseButtonClicked);
    connect(ui->splitter, &QSplitter::splitterMoved, this, [=](int pos, int index) {
        Q_UNUSED(index);
        Q_UNUSED(pos);
        this->m_leftWidth = ui->widgetClient->width();
        qInfo() << "Left width changed:" << this->m_leftWidth;
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

void MqttClientUi::onLogMessageReceived(const QString &msg, bool isError)
{
    ui->textBrowser->append(msg);
}

} // namespace xMQTT