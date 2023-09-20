/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 ******************************************************************************/
#include "sakbroadcastassistant.h"

#include <QDateTime>
#include <QNetworkAddressEntry>
#include <QNetworkInterface>

#include "SAKCommonDataStructure.h"
#include "sakbroadcastthread.h"
#include "ui_sakbroadcastassistant.h"

SAKBroadcastAssistant::SAKBroadcastAssistant(QWidget* parent)
    : QWidget(parent), ui_(new Ui::SAKBroadcastAssistant) {
  ui_->setupUi(this);
  broadcast_thread_ = new SAKBroadcastThread(this);
  connect(broadcast_thread_, &SAKBroadcastThread::started, this,
          [=]() { UpdateUiState(true); });
  connect(broadcast_thread_, &SAKBroadcastThread::finished, this,
          [=]() { UpdateUiState(false); });
  connect(broadcast_thread_, &SAKBroadcastThread::BytesWritten, this,
          [=](const QByteArray& bytes) {
            QByteArray temp = bytes;
            int format = ui_->comboBoxOutputFormat->currentData().toInt();
            auto cookedFormat =
                SAKCommonDataStructure::SAKEnumTextFormatOutput(format);
            auto bytesString =
                SAKCommonDataStructure::byteArrayToString(temp, cookedFormat);
            auto info = QDateTime::currentDateTime().toString("hh:mm:ss");
            info += " Tx: ";
            info = QString("<font color=silver>%1</font>").arg(info);
            info += bytesString;
            ui_->textBrowserInformation->append(info);
          });
  connect(ui_->comboBoxBroadcastFormat, &QComboBox::currentTextChanged, this,
          [=](const QString& text) {
            Q_UNUSED(text);
            SAKCommonDataStructure::setLineEditTextFormat(
                ui_->lineEditBroadcastData,
                ui_->comboBoxBroadcastFormat->currentData().toInt());
          });
  connect(ui_->pushButtonBroadcast, &QPushButton::clicked, this,
          &SAKBroadcastAssistant::OnBroadcastPushButtonClicked);

  InitUi();
  setWindowTitle(tr("Broadcast Assistant"));
}

SAKBroadcastAssistant::~SAKBroadcastAssistant() { delete ui_; }

void SAKBroadcastAssistant::UpdateUiState(bool started) {
  ui_->pushButtonBroadcast->setEnabled(true);
  ui_->pushButtonBroadcast->setText(started ? tr("Terminate")
                                            : tr("Broadcast"));

  ui_->comboBoxBroadcastAddress->setEnabled(!started);
  ui_->lineEditBroadcastPort->setEnabled(!started);
  ui_->comboBoxBroadcastInterval->setEnabled(!started);
  ui_->comboBoxBroadcastPrefix->setEnabled(!started);
  ui_->comboBoxBroadcastSuffix->setEnabled(!started);
  ui_->comboBoxBroadcastFormat->setEnabled(!started);
  ui_->lineEditBroadcastData->setEnabled(!started);
}

void SAKBroadcastAssistant::InitUi() {
  ui_->textBrowserInformation->document()->setMaximumBlockCount(2000);

  InitUiBroadcastAddress();
  InitUiBroadcastInterval();

  SAKCommonDataStructure::setComboBoxTextInputFormat(
      ui_->comboBoxBroadcastFormat);
  SAKCommonDataStructure::setComboBoxTextOutputFormat(
      ui_->comboBoxOutputFormat);
  SAKCommonDataStructure::setupSuffix(ui_->comboBoxBroadcastPrefix);
  SAKCommonDataStructure::setupSuffix(ui_->comboBoxBroadcastSuffix);
}

void SAKBroadcastAssistant::InitUiBroadcastAddress() {
  ui_->comboBoxBroadcastAddress->clear();
  auto bd = QHostAddress(QHostAddress::Broadcast);
  ui_->comboBoxBroadcastAddress->addItem(bd.toString());

  auto interfaces = QNetworkInterface::allInterfaces();
  for (auto& interface : interfaces) {
    auto entries = interface.addressEntries();
    for (auto& entry : entries) {
      auto broadcast_ip = entry.broadcast().toString();
      if (!broadcast_ip.isEmpty()) {
        int count = ui_->comboBoxBroadcastAddress->count();
        bool existed = false;
        for (int i = 0; i < count; i++) {
          auto itemText = ui_->comboBoxBroadcastAddress->itemText(i);
          if (itemText == broadcast_ip) {
            existed = true;
            break;
          }
        }

        if (!existed) {
          ui_->comboBoxBroadcastAddress->addItem(broadcast_ip);
        }
      }
    }
  }
}

void SAKBroadcastAssistant::InitUiBroadcastInterval() {
  ui_->comboBoxBroadcastInterval->clear();
  for (int i = 20; i <= 100; i += 20) {
    ui_->comboBoxBroadcastInterval->addItem(QString::number(i), i);
  }

  for (int i = 200; i <= 1000; i += 200) {
    ui_->comboBoxBroadcastInterval->addItem(QString::number(i), i);
  }

  for (int i = 2000; i <= 10000; i += 2000) {
    ui_->comboBoxBroadcastInterval->addItem(QString::number(i), i);
  }

  ui_->comboBoxBroadcastInterval->setCurrentText("1000");
}

QByteArray SAKBroadcastAssistant::PacketData() {
  QByteArray bytes;

  int prefixType = ui_->comboBoxBroadcastPrefix->currentData().toInt();
  QByteArray prefix = SAKCommonDataStructure::prefix(prefixType).toLatin1();

  int format = ui_->comboBoxBroadcastFormat->currentData().toInt();
  QString text = ui_->lineEditBroadcastData->text();
  QByteArray data = SAKCommonDataStructure::stringToByteArray(text, format);

  int suffixType = ui_->comboBoxBroadcastSuffix->currentData().toInt();
  QByteArray suffix = SAKCommonDataStructure::suffix(suffixType).toLatin1();

  bytes.append(prefix);
  bytes.append(data);
  bytes.append(suffix);
  return bytes;
}

void SAKBroadcastAssistant::OnBroadcastPushButtonClicked() {
  ui_->pushButtonBroadcast->setEnabled(false);

  if (broadcast_thread_->isRunning()) {
    broadcast_thread_->exit();
  } else {
    auto bytes = PacketData();
    if (bytes.isEmpty()) {
      ui_->pushButtonBroadcast->setEnabled(true);
      return;
    }

    broadcast_thread_->SetBroadcastInformation(
        ui_->comboBoxBroadcastAddress->currentText(),
        ui_->lineEditBroadcastPort->text().toInt(),
        ui_->comboBoxBroadcastInterval->currentData().toInt(), bytes);
    broadcast_thread_->start();
  }
}
