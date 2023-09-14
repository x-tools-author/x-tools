/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#include <QDateTime>
#include <QNetworkInterface>
#include <QNetworkAddressEntry>

#include "SAKCommonDataStructure.hh"
#include "SAKToolBroadcastAssistant.h"
#include "ui_SAKToolBroadcastAssistant.h"

SAKToolBroadcastAssistant::SAKToolBroadcastAssistant(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SAKToolBroadcastAssistant)
{
    ui->setupUi(this);
    mBroadcastThread = new SAKToolBroadcastThread(this);
    connect(mBroadcastThread, &SAKToolBroadcastThread::started, this, [=](){
        updateUiState(true);
    });
    connect(mBroadcastThread, &SAKToolBroadcastThread::finished, this, [=](){
        updateUiState(false);
    });
    connect(mBroadcastThread, &SAKToolBroadcastThread::bytesWritten,
            this, [=](const QByteArray &bytes){
        QByteArray temp = bytes;
        int format = ui->comboBoxOutputFormat->currentData().toInt();
        auto cookedFormat =
                SAKCommonDataStructure::SAKEnumTextFormatOutput(format);
        auto bytesString =
                SAKCommonDataStructure::byteArrayToString(temp, cookedFormat);
        auto info = QDateTime::currentDateTime().toString("hh:mm:ss");
        info += " Tx: ";
        info = QString("<font color=silver>%1</font>").arg(info);
        info += bytesString;
        ui->textBrowserInformation->append(info);
    });
    connect(ui->comboBoxBroadcastFormat, &QComboBox::currentTextChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        SAKCommonDataStructure::setLineEditTextFormat(
                    ui->lineEditBroadcastData,
                    ui->comboBoxBroadcastFormat->currentData().toInt());
    });
    connect(ui->pushButtonBroadcast, &QPushButton::clicked,
            this, &SAKToolBroadcastAssistant::onBroadcastPushButtonClicked);

    initUi();
    setWindowTitle(tr("Broadcast Assistant"));
}

SAKToolBroadcastAssistant::~SAKToolBroadcastAssistant()
{
    delete ui;
}

void SAKToolBroadcastAssistant::updateUiState(bool started)
{
    ui->pushButtonBroadcast->setEnabled(true);
    ui->pushButtonBroadcast->setText(started
                                     ? tr("Terminate")
                                     : tr("Broadcast"));

    ui->comboBoxBroadcastAddress->setEnabled(!started);
    ui->lineEditBroadcastPort->setEnabled(!started);
    ui->comboBoxBroadcastInterval->setEnabled(!started);
    ui->comboBoxBroadcastPrefix->setEnabled(!started);
    ui->comboBoxBroadcastSuffix->setEnabled(!started);
    ui->comboBoxBroadcastFormat->setEnabled(!started);
    ui->lineEditBroadcastData->setEnabled(!started);
}

void SAKToolBroadcastAssistant::initUi()
{
    ui->textBrowserInformation->document()->setMaximumBlockCount(2000);

    initUiBroadcastAddress();
    initUiBroadcastInterval();

    SAKCommonDataStructure::setComboBoxTextInputFormat(
                ui->comboBoxBroadcastFormat);
    SAKCommonDataStructure::setComboBoxTextOutputFormat(
                ui->comboBoxOutputFormat);
    SAKCommonDataStructure::setupSuffix(ui->comboBoxBroadcastPrefix);
    SAKCommonDataStructure::setupSuffix(ui->comboBoxBroadcastSuffix);
}

void SAKToolBroadcastAssistant::initUiBroadcastAddress()
{
    ui->comboBoxBroadcastAddress->clear();
    auto bd = QHostAddress(QHostAddress::Broadcast);
    ui->comboBoxBroadcastAddress->addItem(bd.toString());

    auto interfaces = QNetworkInterface::allInterfaces();
    for (auto &interface : interfaces) {
        auto entries = interface.addressEntries();
        for (auto &entry : entries) {
            auto broadcastIp = entry.broadcast().toString();
            if (!broadcastIp.isEmpty()) {
                int count = ui->comboBoxBroadcastAddress->count();
                bool existed = false;
                for (int i = 0; i < count; i++) {
                    auto itemText = ui->comboBoxBroadcastAddress->itemText(i);
                    if (itemText == broadcastIp) {
                        existed = true;
                        break;
                    }
                }

                if (!existed) {
                    ui->comboBoxBroadcastAddress->addItem(broadcastIp);
                }
            }
        }
    }
}

void SAKToolBroadcastAssistant::initUiBroadcastInterval()
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

QByteArray SAKToolBroadcastAssistant::packetData()
{
    QByteArray bytes;

    int prefixType = ui->comboBoxBroadcastPrefix->currentData().toInt();
    QByteArray prefix = SAKCommonDataStructure::prefix(prefixType).toLatin1();

    int format = ui->comboBoxBroadcastFormat->currentData().toInt();
    QString text = ui->lineEditBroadcastData->text();
    QByteArray data = SAKCommonDataStructure::stringToByteArray(text, format);

    int suffixType = ui->comboBoxBroadcastSuffix->currentData().toInt();
    QByteArray suffix = SAKCommonDataStructure::suffix(suffixType).toLatin1();

    bytes.append(prefix);
    bytes.append(data);
    bytes.append(suffix);
    return bytes;
}

void SAKToolBroadcastAssistant::onBroadcastPushButtonClicked()
{
    ui->pushButtonBroadcast->setEnabled(false);

    if (mBroadcastThread->isRunning()) {
        mBroadcastThread->exit();
    } else {
        auto bytes = packetData();
        if (bytes.isEmpty()) {
            ui->pushButtonBroadcast->setEnabled(true);
            return;
        }

        mBroadcastThread->setBroadcastInformation(
                    ui->comboBoxBroadcastAddress->currentText(),
                    ui->lineEditBroadcastPort->text().toInt(),
                    ui->comboBoxBroadcastInterval->currentData().toInt(),
                    bytes);
        mBroadcastThread->start();
    }
}
