/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife
 * project(https://github.com/qsaker/QtSwissArmyKnife).
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in
 * the root of the source code directory.
 *****************************************************************************/
#ifndef SAKTOOLBROADCASTASSISTANT_HH
#define SAKTOOLBROADCASTASSISTANT_HH

#include <QDialog>
#include "SAKToolBroadcastThread.hh"

namespace Ui
{
    class SAKToolBroadcastAssistant;
}

class SAKToolBroadcastAssistant : public QDialog
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKToolBroadcastAssistant(QWidget *parent = Q_NULLPTR);
    ~SAKToolBroadcastAssistant();

private:
    Ui::SAKToolBroadcastAssistant *ui;
    SAKToolBroadcastThread *mBroadcastThread;

private:
    void initUi();
    void initUiBroadcastAddress();
    void initUiBroadcastInterval();
    void updateUiState(bool started);
    QByteArray packetData();

    void onBroadcastPushButtonClicked();
};

#endif // SAKTOOLBROADCASTASSISTANT_HH
