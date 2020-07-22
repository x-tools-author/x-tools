/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom". It is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKDebugPage.hh"
#include "SAKDebugPageStatisticsManager.hh"

SAKDebugPageStatisticsManager::SAKDebugPageStatisticsManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
{
    rxSpeedLabel    = debugPage->mRxSpeedLabel;
    txSpeedLabel    = debugPage->mTxSpeedLabel;
    txFramesLabel   = debugPage->mTxFramesLabel;
    rxFramesLabel   = debugPage->mRxFramesLabel;
    txBytesLabel    = debugPage->mTxBytesLabel;
    rxBytesLabel    = debugPage->mRxBytesLabel;

    resetTxCountPushButton  = debugPage->mResetTxCountPushButton;
    resetRxCountPushButton  = debugPage->mResetRxCountPushButton;

    dataContext.rxBytes  = 0;
    dataContext.txBytes  = 0;
    dataContext.rxFrames = 0;
    dataContext.txFrames = 0;
    dataContext.rxBytesPerSecond = 0;
    dataContext.txBytesPerSecond = 0;

    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKDebugPageStatisticsManager::dataRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKDebugPageStatisticsManager::dataReceived);
    connect(resetRxCountPushButton, &QPushButton::clicked, this, &SAKDebugPageStatisticsManager::clearRxStatistics);
    connect(resetTxCountPushButton, &QPushButton::clicked, this, &SAKDebugPageStatisticsManager::clearTxStatistics);

    speedCalculationTimer.setInterval(1*1000);
    connect(&speedCalculationTimer, &QTimer::timeout, this, &SAKDebugPageStatisticsManager::speedCalculationTimerTimeout);
    speedCalculationTimer.start();
}

void SAKDebugPageStatisticsManager::dataRead(QByteArray data)
{
    dataContext.rxFrames += 1;
    dataContext.rxBytes += static_cast<quint64>(data.length());
    setLabelText(rxFramesLabel, dataContext.rxFrames);
    setLabelText(rxBytesLabel, dataContext.rxBytes);

    dataContext.rxBytesPerSecond += static_cast<quint64>(data.length());
}

void SAKDebugPageStatisticsManager::dataReceived(QByteArray data)
{
    dataContext.txFrames += 1;
    dataContext.txBytes += static_cast<quint64>(data.length());
    setLabelText(txFramesLabel, dataContext.txFrames);
    setLabelText(txBytesLabel, dataContext.txBytes);

    dataContext.txBytesPerSecond += static_cast<quint64>(data.length());
}

void SAKDebugPageStatisticsManager::clearRxStatistics()
{
    dataContext.rxBytes = 0;
    dataContext.rxFrames = 0;
    setLabelText(rxFramesLabel, dataContext.rxFrames);
    setLabelText(rxBytesLabel, dataContext.rxBytes);
}

void SAKDebugPageStatisticsManager::clearTxStatistics()
{
    dataContext.txBytes = 0;
    dataContext.txFrames = 0;
    setLabelText(txFramesLabel, dataContext.txFrames);
    setLabelText(txBytesLabel, dataContext.txBytes);
}

void SAKDebugPageStatisticsManager::setLabelText(QLabel *label, quint64 text)
{
    if (label){
        label->setText(QString::number(text));
    }
}

void SAKDebugPageStatisticsManager::speedCalculationTimerTimeout()
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
