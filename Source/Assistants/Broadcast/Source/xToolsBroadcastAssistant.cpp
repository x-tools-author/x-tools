/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsBroadcastAssistant.h"
#include "ui_xToolsBroadcastAssistant.h"

#include <QDateTime>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>

#include "xToolsApplication.h"
#include "xToolsBroadcastThread.h"
#include "xToolsDataStructure.h"

xToolsBroadcastAssistant::xToolsBroadcastAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xToolsBroadcastAssistant)
{
    ui->setupUi(this);
    m_broadcastThread = new xToolsBroadcastThread(this);
    connect(m_broadcastThread, &xToolsBroadcastThread::started, this, [=]() { updateUiState(true); });
    connect(m_broadcastThread, &xToolsBroadcastThread::finished, this, [=]() { updateUiState(false); });
    connect(m_broadcastThread, &xToolsBroadcastThread::bytesWritten, this, [=](const QByteArray& bytes) {
        QByteArray temp = bytes;
        int format = ui->comboBoxOutputFormat->currentData().toInt();
        auto bytesString = xToolsDataStructure::byteArrayToString(temp, format);
        auto info = QDateTime::currentDateTime().toString("hh:mm:ss");
        info += " Tx: ";
        info = QString("<font color=silver>%1</font>").arg(info);
        info += bytesString;
        ui->textBrowserInformation->append(info);
    });
    connect(ui->comboBoxBroadcastFormat, &QComboBox::currentTextChanged, this, [=]() {
        auto format = ui->comboBoxBroadcastFormat->currentData().toInt();
        xToolsApplication::setValidator(ui->lineEditBroadcastData, format);
    });
    connect(ui->pushButtonBroadcast,
            &QPushButton::clicked,
            this,
            &xToolsBroadcastAssistant::onBroadcastPushButtonClicked);

    initUi();
    setWindowTitle(tr("Broadcast Assistant"));
}

xToolsBroadcastAssistant::~xToolsBroadcastAssistant()
{
    delete ui;
}

void xToolsBroadcastAssistant::updateUiState(bool started)
{
    ui->pushButtonBroadcast->setEnabled(true);
    ui->pushButtonBroadcast->setText(started ? tr("Terminate") : tr("Broadcast"));

    ui->comboBoxBroadcastAddress->setEnabled(!started);
    ui->lineEditBroadcastPort->setEnabled(!started);
    ui->comboBoxBroadcastInterval->setEnabled(!started);
    ui->comboBoxBroadcastPrefix->setEnabled(!started);
    ui->comboBoxBroadcastSuffix->setEnabled(!started);
    ui->comboBoxBroadcastFormat->setEnabled(!started);
    ui->lineEditBroadcastData->setEnabled(!started);
}

void xToolsBroadcastAssistant::initUi()
{
    ui->textBrowserInformation->document()->setMaximumBlockCount(2000);
    initUiBroadcastAddress();
    initUiBroadcastInterval();
}

void xToolsBroadcastAssistant::initUiBroadcastAddress()
{
    ui->comboBoxBroadcastAddress->clear();
    auto bd = QHostAddress(QHostAddress::Broadcast);
    ui->comboBoxBroadcastAddress->addItem(bd.toString());

    auto interfaces = QNetworkInterface::allInterfaces();
    for (auto& interface : interfaces) {
        auto entries = interface.addressEntries();
        for (auto& entry : entries) {
            auto broadcast_ip = entry.broadcast().toString();
            if (!broadcast_ip.isEmpty()) {
                int count = ui->comboBoxBroadcastAddress->count();
                bool existed = false;
                for (int i = 0; i < count; i++) {
                    auto itemText = ui->comboBoxBroadcastAddress->itemText(i);
                    if (itemText == broadcast_ip) {
                        existed = true;
                        break;
                    }
                }

                if (!existed) {
                    ui->comboBoxBroadcastAddress->addItem(broadcast_ip);
                }
            }
        }
    }
}

void xToolsBroadcastAssistant::initUiBroadcastInterval()
{
    ui->comboBoxBroadcastInterval->clear();
    for (int i = 20; i <= 100; i += 20) {
        ui->comboBoxBroadcastInterval->addItem(QString::number(i), i);
    }

    for (int i = 200; i <= 1000; i += 200) {
        ui->comboBoxBroadcastInterval->addItem(QString::number(i), i);
    }

    for (int i = 2000; i <= 10000; i += 2000) {
        ui->comboBoxBroadcastInterval->addItem(QString::number(i), i);
    }

    ui->comboBoxBroadcastInterval->setCurrentText("1000");
}

QByteArray xToolsBroadcastAssistant::packetData()
{
    QByteArray bytes;

    int prefixType = ui->comboBoxBroadcastPrefix->currentData().toInt();
    QByteArray prefix = xToolsDataStructure::affixesData(prefixType);

    int format = ui->comboBoxBroadcastFormat->currentData().toInt();
    QString text = ui->lineEditBroadcastData->text();
    QByteArray data = xToolsDataStructure::stringToByteArray(text, format);

    int suffixType = ui->comboBoxBroadcastSuffix->currentData().toInt();
    QByteArray suffix = xToolsDataStructure::affixesData(suffixType);

    bytes.append(prefix);
    bytes.append(data);
    bytes.append(suffix);
    return bytes;
}

void xToolsBroadcastAssistant::onBroadcastPushButtonClicked()
{
    ui->pushButtonBroadcast->setEnabled(false);

    if (m_broadcastThread->isRunning()) {
        m_broadcastThread->exit();
    } else {
        auto bytes = packetData();
        if (bytes.isEmpty()) {
            ui->pushButtonBroadcast->setEnabled(true);
            return;
        }

        m_broadcastThread
            ->setBroadcastInformation(ui->comboBoxBroadcastAddress->currentText(),
                                      ui->lineEditBroadcastPort->text().toInt(),
                                      ui->comboBoxBroadcastInterval->currentData().toInt(),
                                      bytes);
        m_broadcastThread->start();
    }
}
