/***************************************************************************************************
 * Copyright 2023-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKBROADCASTASSISTANT_H
#define SAKBROADCASTASSISTANT_H

#include <QWidget>

namespace Ui {
class SAKBroadcastAssistant;
}

class SAKBroadcastThread;
class SAKBroadcastAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKBroadcastAssistant(QWidget* parent = Q_NULLPTR);
    ~SAKBroadcastAssistant();

private:
    Ui::SAKBroadcastAssistant* ui;
    SAKBroadcastThread* m_broadcastThread;

private:
    void initUi();
    void initUiBroadcastAddress();
    void initUiBroadcastInterval();
    void updateUiState(bool started);
    QByteArray packetData();

private slots:
    void onBroadcastPushButtonClicked();
};

#endif // SAKBROADCASTASSISTANT_H
