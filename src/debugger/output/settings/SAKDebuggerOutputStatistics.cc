/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include "SAKDebugPage.hh"
#include "SAKStatisticsManager.hh"

SAKStatisticsManager::SAKStatisticsManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
{
    rxSpeedLabel    = debugPage->rxSpeedLabel;
    txSpeedLabel    = debugPage->txSpeedLabel;
    txFramesLabel   = debugPage->txFramesLabel;
    rxFramesLabel   = debugPage->rxFramesLabel;
    txBytesLabel    = debugPage->txBytesLabel;
    rxBytesLabel    = debugPage->rxBytesLabel;

    resetTxCountPushButton  = debugPage->resetTxCountPushButton;
    resetRxCountPushButton  = debugPage->resetRxCountPushButton;

    dataContext.rxBytes  = 0;
    dataContext.txBytes  = 0;
    dataContext.rxFrames = 0;
    dataContext.txFrames = 0;
    dataContext.rxBytesPerSecond = 0;
    dataContext.txBytesPerSecond = 0;

    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKStatisticsManager::dataRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKStatisticsManager::dataReceived);
    connect(resetRxCountPushButton, &QPushButton::clicked, this, &SAKStatisticsManager::clearRxStatistics);
    connect(resetTxCountPushButton, &QPushButton::clicked, this, &SAKStatisticsManager::clearTxStatistics);

    speedCalculationTimer.setInterval(1*1000);
    connect(&speedCalculationTimer, &QTimer::timeout, this, &SAKStatisticsManager::speedCalculationTimerTimeout);
    speedCalculationTimer.start();
}

void SAKStatisticsManager::dataRead(QByteArray data)
{
    dataContext.rxFrames += 1;
    dataContext.rxBytes += static_cast<quint64>(data.length());
    setLabelText(rxFramesLabel, dataContext.rxFrames);
    setLabelText(rxBytesLabel, dataContext.rxBytes);

    dataContext.rxBytesPerSecond += static_cast<quint64>(data.length());
}

void SAKStatisticsManager::dataReceived(QByteArray data)
{
    dataContext.txFrames += 1;
    dataContext.txBytes += static_cast<quint64>(data.length());
    setLabelText(txFramesLabel, dataContext.txFrames);
    setLabelText(txBytesLabel, dataContext.txBytes);

    dataContext.txBytesPerSecond += static_cast<quint64>(data.length());
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

void SAKStatisticsManager::speedCalculationTimerTimeout()
{
    auto cal = [](QLabel *label, quint64 &bytes){
        if (bytes < 1024){
            label->setText(QString("%1 B/s").arg(bytes));
        }else if(bytes < (1024*1024)){
            label->setText(QString("%1 KB/s").arg(bytes/1024));
        }else{
            label->setText(QString("%1 MB/s").arg(bytes/(1024*1024)));
        }

        bytes = 0;
    };

    cal(rxSpeedLabel, dataContext.rxBytesPerSecond);
    cal(txSpeedLabel, dataContext.txBytesPerSecond);
}
