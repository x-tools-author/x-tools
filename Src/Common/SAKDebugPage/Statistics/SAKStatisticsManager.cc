/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include "ui_SAKDebugPage.h"
#include "SAKStatisticsManager.hh"

SAKStatisticsManager::SAKStatisticsManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,_debugPage (debugPage)
{
    txFramesLabel   = _debugPage->ui->txFramesLabel;
    rxFramesLabel   = _debugPage->ui->rxFramesLabel;
    txBytesLabel    = _debugPage->ui->txBytesLabel;
    rxBytesLabel    = _debugPage->ui->rxBytesLabel;

    resetTxCountPushButton  = _debugPage->ui->resetTxCountPushButton;
    resetRxCountPushButton  = _debugPage->ui->resetRxCountPushButton;

    dataContext.rxBytes  = 0;
    dataContext.txBytes  = 0;
    dataContext.rxFrames = 0;
    dataContext.txFrames = 0;

    connect(_debugPage, &SAKDebugPage::dataReadOrwritten, this, &SAKStatisticsManager::dataReadOrwritten);
    connect(resetRxCountPushButton, &QPushButton::clicked, this, &SAKStatisticsManager::clearRxStatistics);
    connect(resetTxCountPushButton, &QPushButton::clicked, this, &SAKStatisticsManager::clearTxStatistics);
}

void SAKStatisticsManager::dataReadOrwritten(QByteArray data, SAKDebugPage::OutputParameters parameters)
{
    if (parameters.isReceivedData){
        dataContext.rxFrames += 1;
        dataContext.rxBytes += static_cast<quint64>(data.length());
        setLabelText(rxFramesLabel, dataContext.rxFrames);
        setLabelText(rxBytesLabel, dataContext.rxBytes);
    }else{
        dataContext.txFrames += 1;
        dataContext.txBytes += static_cast<quint64>(data.length());
        setLabelText(txFramesLabel, dataContext.txFrames);
        setLabelText(txBytesLabel, dataContext.txBytes);
    }
}

void SAKStatisticsManager::clearRxStatistics()
{
    dataContext.rxBytes = 0;
    dataContext.rxFrames = 0;
    setLabelText(rxFramesLabel, dataContext.rxFrames);
    setLabelText(rxBytesLabel, dataContext.rxBytes);
}

void SAKStatisticsManager::clearTxStatistics()
{
    dataContext.txBytes = 0;
    dataContext.txFrames = 0;
    setLabelText(txFramesLabel, dataContext.txFrames);
    setLabelText(txBytesLabel, dataContext.txBytes);
}

void SAKStatisticsManager::setLabelText(QLabel *label, quint64 text)
{
    if (label){
        label->setText(QString::number(text));
    }
}
