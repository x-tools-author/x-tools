/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "mqttserverui.h"
#include "ui_mqttserverui.h"

#include <QApplication>

#include "mqttcommon.h"
#include "mqttdatamodel.h"
#include "mqttserver.h"
#include "utilities/x.h"

namespace xMQTT {

struct MqttServerUiParameterKeys
{
    const QString rightWidth{"rightWidth"};
    const QString serverAddress{"serverAddress"};
    const QString serverPort{"serverPort"};
    const QString tabIndex{"tabIndex"};

    const QString logPage{"logPage"};
    const QString messagePage{"messagePage"};
};

MqttServerUi::MqttServerUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttServerUi)
{
    ui->setupUi(this);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes(QList<int>({width() - m_rightWidth, m_rightWidth}));
    ui->pushButtonClose->setEnabled(false);
    ui->treeViewClients->header()->hide();
    setupSocketAddress(ui->comboBoxServerAddress);
    setupPortSpinBox(ui->spinBoxServerPort);
    xSetNoneBorder(ui->treeViewClients);
    connect(ui->pushButtonOpen, &QPushButton::clicked, this, &MqttServerUi::onOpenBtnClicked);
    connect(ui->pushButtonClose, &QPushButton::clicked, this, &MqttServerUi::onCloseBtnClicked);
    connect(ui->splitter, &QSplitter::splitterMoved, this, [this](int pos, int index) {
        Q_UNUSED(index);
        Q_UNUSED(pos);
        this->m_rightWidth = ui->widgetServer->width();
    });

    m_clientsModel = new QStandardItemModel(this);
    ui->treeViewClients->setModel(m_clientsModel);

    m_server = new MqttServer(this);
    connect(m_server, &MqttServer::logMessage, this, &MqttServerUi::onLogMessageReceived);
    connect(m_server, &MqttServer::mqttMessageRx, this, &MqttServerUi::onMqttMessageRx);
    connect(m_server, &MqttServer::serverStarted, this, &MqttServerUi::onServerStarted);
    connect(m_server, &MqttServer::finished, this, &MqttServerUi::onServerStopped);
    connect(m_server, &MqttServer::clientConnected, this, &MqttServerUi::onClientConnected);
    connect(m_server, &MqttServer::clientDisconnected, this, &MqttServerUi::onClientDisconnected);
    connect(m_server, &MqttServer::clientSubscribed, this, &MqttServerUi::onClientSubscribed);
    connect(m_server, &MqttServer::clientUnsubscribed, this, &MqttServerUi::onClientUnsubscribed);

#if 1
    ui->tabWidget->removeTab(1); // Remove Message tab
    disconnect(m_server, &MqttServer::mqttMessageRx, this, &MqttServerUi::onMqttMessageRx);
#endif
}

MqttServerUi::~MqttServerUi()
{
    delete ui;
}

QJsonObject MqttServerUi::save()
{
    const MqttServerUiParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.rightWidth, static_cast<int>(m_rightWidth));
    obj.insert(keys.serverAddress, ui->comboBoxServerAddress->currentText());
    obj.insert(keys.serverPort, ui->spinBoxServerPort->value());
    obj.insert(keys.tabIndex, ui->tabWidget->currentIndex());
    obj.insert(keys.logPage, ui->widgetLog->save());
    obj.insert(keys.messagePage, ui->widgetMessage->save());
    return obj;
}

void MqttServerUi::load(const QJsonObject &obj)
{
    const MqttServerUiParameterKeys keys;
    const int rightWidth = obj.value(keys.rightWidth).toInt(168);
    m_rightWidth = rightWidth;
    ui->splitter->setSizes(QList<int>({width() - m_rightWidth, m_rightWidth}));
    const QString serverAddress = obj.value(keys.serverAddress).toString("127.0.0.1");
    ui->comboBoxServerAddress->setCurrentText(serverAddress);
    ui->spinBoxServerPort->setValue(obj.value(keys.serverPort).toInt());
    ui->tabWidget->setCurrentIndex(obj.value(keys.tabIndex).toInt(0));

    ui->widgetLog->load(obj.value(keys.logPage).toObject());
    ui->widgetMessage->load(obj.value(keys.messagePage).toObject());
}

bool MqttServerUi::event(QEvent *event)
{
    if (ui && event->type() == QEvent::Resize) {
        ui->splitter->setSizes({width() - m_rightWidth, m_rightWidth});
    }

    return QWidget::event(event);
}

QString MqttServerUi::clientFlag(const QString &ip, quint16 port)
{
    return QString("%1:%2").arg(ip).arg(port);
}

QStandardItem *MqttServerUi::findClientItem(const QString &ip, quint16 port)
{
    const QString flag = clientFlag(ip, port);
    QList<QStandardItem *> items = m_clientsModel->findItems(flag);
    if (items.isEmpty()) {
        return nullptr;
    }
    return items.first();
}

QStandardItem *MqttServerUi::addClientItem(const QString &ip, quint16 port)
{
    const QString flag = clientFlag(ip, port);
    QStandardItem *item = new QStandardItem(flag);
    m_clientsModel->appendRow(item);
    return item;
}

void MqttServerUi::removeClientItem(const QString &ip, quint16 port)
{
    QStandardItem *item = findClientItem(ip, port);
    if (item) {
        m_clientsModel->removeRow(item->row());
    }
}

QStandardItem *MqttServerUi::findSubscriptionItem(QStandardItem *clientItem, const QString &topic)
{
    if (!clientItem) {
        return nullptr;
    }

    for (int i = 0; i < clientItem->rowCount(); ++i) {
        QStandardItem *subItem = clientItem->child(i);
        if (subItem && subItem->text() == topic) {
            return subItem;
        }
    }

    return nullptr;
}

QStandardItem *MqttServerUi::addSubscriptionItem(QStandardItem *clientItem, const QString &topic)
{
    if (!clientItem) {
        return nullptr;
    }

    QStandardItem *subItem = new QStandardItem(topic);
    clientItem->appendRow(subItem);
    return subItem;
}

void MqttServerUi::removeSubscriptionItem(QStandardItem *clientItem, const QString &topic)
{
    QStandardItem *subItem = findSubscriptionItem(clientItem, topic);
    if (subItem) {
        clientItem->removeRow(subItem->row());
    }
}

void MqttServerUi::disableOpenBtnAndCloseBtn()
{
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(false);
    QApplication::processEvents(); // Make buttons status updated
}

void MqttServerUi::onOpenBtnClicked()
{
    if (!m_server) {
        return;
    }

    disableOpenBtnAndCloseBtn();
    const QString ip = ui->comboBoxServerAddress->currentText();
    const quint16 port = static_cast<quint16>(ui->spinBoxServerPort->value());
    m_server->startServer(ip, port);
}

void MqttServerUi::onCloseBtnClicked()
{
    disableOpenBtnAndCloseBtn();
    if (m_server) {
        m_server->stopServer();
    }
}

void MqttServerUi::onLogMessageReceived(const QString &msg, bool isError)
{
    Q_UNUSED(isError);
    ui->widgetLog->appendLogMessage(msg, isError);

    if (isError) {
        onCloseBtnClicked();
        QApplication::beep();
    }
}

void MqttServerUi::onMqttMessageRx(std::shared_ptr<MqttMessage> message)
{
    ui->widgetMessage->model()->addMessage(message);
}

void MqttServerUi::onServerStarted()
{
    ui->pushButtonClose->setEnabled(true);
    ui->pushButtonOpen->setEnabled(false);
}

void MqttServerUi::onServerStopped()
{
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(true);
}

void MqttServerUi::onClientConnected(const QString &ip, quint16 port)
{
    QStandardItem *clientItem = findClientItem(ip, port);
    if (!clientItem) {
        addClientItem(ip, port);
    }

    ui->treeViewClients->expandAll();
}

void MqttServerUi::onClientDisconnected(const QString &ip, quint16 port)
{
    removeClientItem(ip, port);
}

void MqttServerUi::onClientSubscribed(const QString &ip, quint16 port, const QString &topic)
{
    const QString flag = clientFlag(ip, port);
    QStandardItem *clientItem = findClientItem(ip, port);
    if (!clientItem) {
        clientItem = addClientItem(ip, port);
        addSubscriptionItem(clientItem, topic);
    } else {
        QStandardItem *subItem = findSubscriptionItem(clientItem, topic);
        if (!subItem) {
            addSubscriptionItem(clientItem, topic);
        }
    }

    ui->treeViewClients->expandAll();
}

void MqttServerUi::onClientUnsubscribed(const QString &ip, quint16 port, const QString &topic)
{
    QStandardItem *clientItem = findClientItem(ip, port);
    if (clientItem) {
        removeSubscriptionItem(clientItem, topic);
    }
}

} // namespace xMQTT