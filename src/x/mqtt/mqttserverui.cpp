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

    m_server = new MqttServer(this);
    connect(m_server, &MqttServer::logMessage, this, &MqttServerUi::onLogMessageReceived);
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

void MqttServerUi::onOpenBtnClicked()
{
    if (!m_server) {
        return;
    }

    const QString ip = ui->comboBoxServerAddress->currentText();
    const quint16 port = static_cast<quint16>(ui->spinBoxServerPort->value());
    m_server->startServer(ip, port);
}

void MqttServerUi::onCloseBtnClicked()
{
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

} // namespace xMQTT