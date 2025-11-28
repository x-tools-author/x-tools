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
};

MqttServerUi::MqttServerUi(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::MqttServerUi)
{
    ui->setupUi(this);
    ui->splitter->setChildrenCollapsible(false);
    ui->splitter->setSizes(QList<int>({width() - m_rightWidth, m_rightWidth}));
    ui->textBrowserLog->setWordWrapMode(QTextOption::NoWrap);
    ui->textBrowserLog->document()->setMaximumBlockCount(1024);
    ui->pushButtonClose->setEnabled(false);
    ui->treeViewClients->header()->hide();
    setupSocketAddress(ui->comboBoxServerAddress);
    setupPortSpinBox(ui->spinBoxServerPort);
    xSetNoneBorder(ui->treeViewClients);
    xSetNoneBorder(ui->treeViewTopics);
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
    connect(m_server, &MqttServer::clientSubscribed, this, &MqttServerUi::onClientSubscribed);
    connect(m_server, &MqttServer::clientUnsubscribed, this, &MqttServerUi::onClientUnsubscribed);
}

MqttServerUi::~MqttServerUi()
{
    delete ui;
}

QJsonObject MqttServerUi::save()
{
    QJsonObject obj;
    obj.insert(MqttServerUiParameterKeys().rightWidth, static_cast<int>(m_rightWidth));
    return obj;
}

void MqttServerUi::load(const QJsonObject &obj)
{
    int rightWidth = obj.value(MqttServerUiParameterKeys().rightWidth).toInt(168);
    m_rightWidth = rightWidth;
    ui->splitter->setSizes(QList<int>({width() - m_rightWidth, m_rightWidth}));
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

void MqttServerUi::onOpenBtnClicked()
{
    if (!m_server) {
        return;
    }

    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(false);

    const QString ip = ui->comboBoxServerAddress->currentText();
    const quint16 port = static_cast<quint16>(ui->spinBoxServerPort->value());
    m_server->startServer(ip, port);
}

void MqttServerUi::onCloseBtnClicked()
{
    ui->pushButtonClose->setEnabled(false);
    ui->pushButtonOpen->setEnabled(false);

    if (m_server) {
        m_server->stopServer();
    }
}

void MqttServerUi::onLogMessageReceived(const QString &msg, bool isError)
{
    Q_UNUSED(isError);
    ui->textBrowserLog->append(msg);

    if (isError) {
        onCloseBtnClicked();
        QApplication::beep();
    }
}

void MqttServerUi::onMqttMessageRx(std::shared_ptr<MqttMessage> message)
{
    ui->widgetMqttDataView->model()->addMessage(message);
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