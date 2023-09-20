/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKBROADCASTASSISTANT_H
#define SAKBROADCASTASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKBroadcastAssistant;
}

class SAKBroadcastAssistant : public QWidget {
  Q_OBJECT
 public:
  Q_INVOKABLE SAKBroadcastAssistant(QWidget* parent = Q_NULLPTR);
  ~SAKBroadcastAssistant();

 private:
  Ui::SAKBroadcastAssistant* ui;
  SAKBroadcastThread* mBroadcastThread;

 private:
  void initUi();
  void initUiBroadcastAddress();
  void initUiBroadcastInterval();
  void updateUiState(bool started);
  QByteArray packetData();

  void onBroadcastPushButtonClicked();
};

#endif  // SAKBROADCASTASSISTANT_H
