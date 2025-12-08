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

#include <QJsonArray>
#include <QMessageBox>

#include "common/xtools.h"
#include "utilities/iconengine.h"

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
    const QString tabIndex{"tabIndex"};
    // Publish panel
    const QString publishTopic{"publishTopic"};
    const QString publicMessage{"message"};
    const QString wrapText{"wrapText"};
    const QString leftTabIndex{"leftTabIndex"};
    const QString interval{"interval"};
    // Subscribe panel
    const QString subscribeTopic{"subscribeTopic"};
    const QString topics{"topics"};
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
    xSetNoneBorder(ui->widgetLog);
    xSetupSocketAddress(ui->comboBoxServerAddress);
    setupPortSpinBox(ui->spinBoxServerPort);
    setupQosComboBox(ui->comboBoxQos);
    setupVersionComboBox(ui->comboBoxVersions);
    setupMessageFormatComboBox(ui->comboBoxTextFormat);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes(QList<int>({m_leftWidth, width() - m_leftWidth}));
    ui->toolButtonPublish->setIcon(xIcon(":/res/icons/upload.svg"));
    ui->toolButtonTopicSubscribe->setIcon(xIcon(":/res/icons/add.svg"));
    ui->toolButtonTopicClear->setIcon(xIcon(":/res/icons/mop.svg"));
    ui->toolButtonTopicRemove->setIcon(xIcon(":/res/icons/remove.svg"));
    ui->toolButtonWrap->setIcon(xIcon(":/res/icons/wrap_text.svg"));
    ui->toolButtonTimer->setIcon(xIcon(":/res/icons/timer.svg"));
    ui->toolButtonTimer->setCheckable(true);
    ui->toolButtonWrap->setCheckable(true);
    ui->toolButtonWrap->setChecked(true);
    ui->pushButtonClose->setEnabled(false);
    // clang-format off
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MqttClientUi::onOpenButtonClicked);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &MqttClientUi::onCloseButtonClicked);
    connect(ui->toolButtonPublish, &QToolButton::clicked, this, &MqttClientUi::onPublishButtonClicked);
    connect(ui->toolButtonTopicSubscribe, &QToolButton::clicked, this, &MqttClientUi::onSubscribeButtonClicked);
    connect(ui->toolButtonTopicRemove, &QToolButton::clicked, this, &MqttClientUi::onRemoveTopicButtonClicked);
    connect(ui->toolButtonTopicClear, &QToolButton::clicked, this, &MqttClientUi::onClearTopicsButtonClicked);
    connect(ui->toolButtonTimer, &QToolButton::clicked, this, &MqttClientUi::onTimerButtonClicked);
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
    connect(m_client, &MqttClient::finished, this, &MqttClientUi::onFinished);
    connect(m_client, &MqttClient::connected, this, &MqttClientUi::onConnected);
    connect(m_client, &MqttClient::disconnected, this, &MqttClientUi::onDisconnected);

    m_topicsModel = new QStandardItemModel(this);
    ui->listViewSubscribe->setModel(m_topicsModel);
#if 1
    ui->tabWidget->removeTab(1); // Remove Message tab
#endif
}

MqttClientUi::~MqttClientUi()
{
    if (m_publishingTimer && m_publishingTimer->isActive()) {
        m_publishingTimer->stop();
    }

    delete ui;
}

QJsonObject MqttClientUi::save() const
{
    MqttClientUiParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.serverAddress, ui->comboBoxServerAddress->currentText());
    obj.insert(keys.serverPort, ui->spinBoxServerPort->value());
    obj.insert(keys.qos, ui->comboBoxQos->currentIndex());
    obj.insert(keys.versions, ui->comboBoxVersions->currentIndex());
    obj.insert(keys.keepAlive, ui->spinBoxKeepAlive->value());
    obj.insert(keys.leftWidth, ui->widgetClient->width());
    obj.insert(keys.tabIndex, ui->tabWidget->currentIndex());
    // Publish panel
    obj.insert(keys.publishTopic, ui->lineEditPublish->text());
    obj.insert(keys.publicMessage, ui->textEditPublish->toPlainText());
    obj.insert(keys.wrapText, ui->toolButtonWrap->isChecked());
    obj.insert(keys.leftTabIndex, ui->tabWidgetLeft->currentIndex());
    obj.insert(keys.interval, ui->spinBoxInterval->value());
    // Subscribe panel
    obj.insert(keys.subscribeTopic, ui->lineEditSubscribe->text());
    QJsonArray topicsArray;
    for (int row = 0; row < m_topicsModel->rowCount(); ++row) {
        QStandardItem *item = m_topicsModel->item(row);
        if (item) {
            topicsArray.append(item->text());
        }
    }
    obj.insert(keys.topics, topicsArray);
    return obj;
}

void MqttClientUi::load(const QJsonObject &obj) const
{
    const MqttClientUiParameterKeys keys;
    const QString address = obj.value(keys.serverAddress).toString();
    ui->comboBoxServerAddress->setCurrentText(address);
    ui->spinBoxServerPort->setValue(obj.value(keys.serverPort).toInt(1883));
    int index = obj.value(keys.qos).toInt(static_cast<int>(QoS::AtMostOnce));
    ui->comboBoxQos->setCurrentIndex(index >= 0 && index < ui->comboBoxQos->count() ? index : 0);
    int leftWidth = obj.value(keys.leftWidth).toInt(m_leftWidth);
    ui->splitter->setSizes({leftWidth, width() - leftWidth});
    int version = obj.value(keys.versions).toInt(static_cast<int>(4));
    index = ui->comboBoxVersions->findData(version);
    ui->comboBoxVersions->setCurrentIndex(index == -1 ? 0 : index);
    ui->spinBoxKeepAlive->setValue(obj.value(keys.keepAlive).toInt(5));
    ui->tabWidget->setCurrentIndex(obj.value(keys.tabIndex).toInt(0));

    // Publish panel
    ui->lineEditPublish->setText(obj.value(keys.publishTopic).toString());
    ui->textEditPublish->setPlainText(obj.value(keys.publicMessage).toString());
    bool wrapText = obj.value(keys.wrapText).toBool(true);
    ui->toolButtonWrap->setChecked(wrapText);
    ui->textEditPublish->setLineWrapMode(wrapText ? QTextEdit::WidgetWidth : QTextEdit::NoWrap);
    index = obj.value(keys.leftTabIndex).toInt(0);
    ui->tabWidgetLeft->setCurrentIndex(index);
    ui->spinBoxInterval->setValue(obj.value(keys.interval).toInt(1000));

    // Subscribe panel
    ui->lineEditSubscribe->setText(obj.value(keys.subscribeTopic).toString());
    m_topicsModel->clear();
    QJsonArray topicsArray = obj.value(keys.topics).toArray();
    for (const QJsonValue &val : topicsArray) {
        QString topic = val.toString();
        if (!topic.isEmpty()) {
            QStandardItem *item = new QStandardItem(topic);
            m_topicsModel->appendRow(item);
        }
    }
}

bool MqttClientUi::event(QEvent *event)
{
    if (ui && event->type() == QEvent::Resize) {
        ui->splitter->setSizes({m_leftWidth, width() - m_leftWidth});
    }

    return QWidget::event(event);
}

void MqttClientUi::onOpenButtonClicked() const
{
    if (!m_client) {
        return;
    }

    disableOpenBtnAndCloseBtn();
    const QString host = ui->comboBoxServerAddress->currentText();
    const auto port = static_cast<quint16>(ui->spinBoxServerPort->value());
    const int qos = ui->comboBoxQos->currentData().toInt();
    const int version = ui->comboBoxVersions->currentData().toInt();
    const int keepAlive = ui->spinBoxKeepAlive->value();
    m_client->startClient(host, port, qos, version, keepAlive);
}

void MqttClientUi::onCloseButtonClicked() const
{
    disableOpenBtnAndCloseBtn();
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
    const QString topic = ui->lineEditSubscribe->text().trimmed();
    if (topic.isEmpty()) {
        showEmptyTopicWarning();
        ui->lineEditSubscribe->setFocus();
        return;
    }

    if (!m_client || (m_client && !m_client->isOpened())) {
        for (int i = 0; i < m_topicsModel->rowCount(); ++i) {
            QStandardItem *existingItem = m_topicsModel->item(i);
            if (existingItem && existingItem->text() == topic) {
                return; // Topic already exists
            }
        }

        QStandardItem *item = new QStandardItem(topic);
        m_topicsModel->appendRow(item);
        return;
    }

    QStandardItem *item = new QStandardItem(topic);
    m_topicsModel->appendRow(item);
    m_client->subscribe(ui->lineEditSubscribe->text().trimmed());
}

void MqttClientUi::onRemoveTopicButtonClicked()
{
    QModelIndexList selectedIndexes = ui->listViewSubscribe->selectionModel()->selectedIndexes();
    for (const QModelIndex &index : selectedIndexes) {
        m_topicsModel->removeRow(index.row());
    }
}

void MqttClientUi::onClearTopicsButtonClicked()
{
    if (m_topicsModel->rowCount() == 0) {
        return;
    }

    int ret = QMessageBox::question(this,
                                    tr("Clear Topics"),
                                    tr("Are you sure you want to clear all subscribed topics?"),
                                    QMessageBox::Yes | QMessageBox::No);
    if (ret == QMessageBox::Yes) {
        m_topicsModel->clear();
    }
}

void MqttClientUi::onTimerButtonClicked(bool checked)
{
    // If client is not opened, disable timer start
    if (!m_client || (m_client && !m_client->isOpened())) {
        showNotOpenedWarning();
        ui->toolButtonTimer->setChecked(false);
        return;
    }

    if (checked) {
        startPublishingTimer();
    } else {
        stopPublishingTimer();
    }
}

void MqttClientUi::onLogMessageReceived(const QString &msg, bool isError) const
{
    ui->widgetLog->appendLogMessage(msg, isError);
    if (isError && m_client) {
        m_client->stopClient();
    }
}

void MqttClientUi::onPublishingTimerTimeout()
{
    if (!m_client || !m_client->isOpened()) {
        return;
    }

    if (ui->lineEditPublish->text().trimmed().isEmpty()) {
        return;
    }

    QString topic = ui->lineEditPublish->text().trimmed();
    QString txt = ui->textEditPublish->toPlainText();
    int data = ui->comboBoxTextFormat->currentData().toInt();
    QByteArray msg = cookedMessage(txt, static_cast<MessageFormat>(data));
    m_client->publish(topic, msg);
}

void MqttClientUi::onFinished()
{
    ui->pushButtonOpen->setEnabled(true);
    ui->pushButtonClose->setEnabled(false);

    ui->toolButtonTopicClear->setEnabled(true);
    ui->toolButtonTopicRemove->setEnabled(true);
}

void MqttClientUi::onConnected()
{
    ui->pushButtonClose->setEnabled(true);
    ui->pushButtonOpen->setEnabled(false);

    ui->toolButtonTopicClear->setEnabled(false);
    ui->toolButtonTopicRemove->setEnabled(false);

    // Subscribe to topics in the list
    for (int row = 0; row < m_topicsModel->rowCount(); ++row) {
        QStandardItem *item = m_topicsModel->item(row);
        if (item) {
            m_client->subscribe(item->text());
        }
    }
}

void MqttClientUi::onDisconnected()
{
    ui->pushButtonOpen->setEnabled(true);
    ui->pushButtonClose->setEnabled(false);

    ui->toolButtonTopicClear->setEnabled(true);
    ui->toolButtonTopicRemove->setEnabled(true);
    m_client->stopClient();
}

void MqttClientUi::showNotOpenedWarning()
{
    QMessageBox::warning(this, tr("Warning"), tr("The MQTT client is not opened."));
}

void MqttClientUi::showEmptyTopicWarning()
{
    QMessageBox::warning(this, tr("Warning"), tr("The topic is empty."));
}

void MqttClientUi::startPublishingTimer()
{
    if (!m_publishingTimer) {
        m_publishingTimer = new QTimer(this);
        connect(m_publishingTimer, &QTimer::timeout, this, &MqttClientUi::onPublishingTimerTimeout);
    }

    int interval = ui->spinBoxInterval->value();
    m_publishingTimer->start(interval < 100 ? 100 : interval); // Ensure minimum interval of 100ms
}

void MqttClientUi::stopPublishingTimer()
{
    if (m_publishingTimer) {
        m_publishingTimer->stop();
    }
}

void MqttClientUi::disableOpenBtnAndCloseBtn() const
{
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(false);
    QApplication::processEvents();
}

} // namespace xMQTT