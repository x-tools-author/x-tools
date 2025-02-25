/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QWidget>

namespace Ui {
class BroadcastAssistant;
}

class BroadcastThread;
class BroadcastAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE BroadcastAssistant(QWidget* parent = Q_NULLPTR);
    ~BroadcastAssistant();

private:
    Ui::BroadcastAssistant* ui;
    BroadcastThread* m_broadcastThread;

private:
    void initUi();
    void initUiBroadcastAddress();
    void initUiBroadcastInterval();
    void updateUiState(bool started);
    QByteArray packetData();

private slots:
    void onBroadcastPushButtonClicked();
};
