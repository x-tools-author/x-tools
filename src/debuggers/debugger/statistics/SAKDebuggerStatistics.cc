/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QTimer>

#include "SAKDebuggerStatistics.hh"

SAKDebuggerStatistics::SAKDebuggerStatistics(QLabel *txSpeed,
                                             QLabel *rxSpeed,
                                             QLabel *txFrame,
                                             QLabel *rxFrame,
                                             QLabel *txBytes,
                                             QLabel *rxBytes,
                                             QObject *parent)
    :QObject(parent)
    ,mTxSpeedLabel(txSpeed)
    ,mRxSpeedLabel(rxSpeed)
    ,mTxFramesLabel(txFrame)
    ,mRxFramesLabel(rxFrame)
    ,mTxBytesLabel(txBytes)
    ,mRxBytesLabel(rxBytes)
{
    mParametersContext.rxBytes = 0;
    mParametersContext.txBytes = 0;
    mParametersContext.rxFrames = 0;
    mParametersContext.txFrames = 0;
    mParametersContext.rxSpeed = 0;
    mParametersContext.txSpeed = 0;


    // update speed
    QTimer *speedTimer = new QTimer(this);
    speedTimer->setInterval(1*1000);
    connect(speedTimer, &QTimer::timeout,
            this, &SAKDebuggerStatistics::updateSpeed);
    speedTimer->start();
}

void SAKDebuggerStatistics::onBytesRead(QByteArray data)
{
    mParametersContext.rxFrames += 1;
    mParametersContext.rxBytes += static_cast<quint64>(data.length());
    setLabelText(mRxFramesLabel, mParametersContext.rxFrames);
    setLabelText(mRxBytesLabel, mParametersContext.rxBytes);

    mParametersContext.rxSpeed += static_cast<quint64>(data.length());
}

void SAKDebuggerStatistics::onBytesWritten(QByteArray data)
{
    mParametersContext.txFrames += 1;
    mParametersContext.txBytes += static_cast<quint64>(data.length());
    setLabelText(mTxFramesLabel, mParametersContext.txFrames);
    setLabelText(mTxBytesLabel, mParametersContext.txBytes);

    mParametersContext.txSpeed += static_cast<quint64>(data.length());
}

void SAKDebuggerStatistics::clearRxStatistics()
{
    mParametersContext.rxBytes = 0;
    mParametersContext.rxFrames = 0;
    setLabelText(mRxFramesLabel, mParametersContext.rxFrames);
    setLabelText(mRxBytesLabel, mParametersContext.rxBytes);
}

void SAKDebuggerStatistics::clearTxStatistics()
{
    mParametersContext.txBytes = 0;
    mParametersContext.txFrames = 0;
    setLabelText(mTxFramesLabel, mParametersContext.txFrames);
    setLabelText(mTxBytesLabel, mParametersContext.txBytes);
}

void SAKDebuggerStatistics::updateSpeed()
{
    auto toSpeedString = [](QLabel *label, quint64 &bytes){
        if (bytes < 1024) {
            label->setText(QString("%1 B/s").arg(bytes));
        } else if (bytes < (1024*1024)) {
            label->setText(QString("%1 KB/s").arg(bytes/1024));
        } else {
            label->setText(QString("%1 MB/s").arg(bytes/(1024*1024)));
        }

        bytes = 0;
    };

    toSpeedString(mRxSpeedLabel, mParametersContext.rxSpeed);
    toSpeedString(mTxSpeedLabel, mParametersContext.txSpeed);
}

void SAKDebuggerStatistics::setLabelText(QLabel *label, quint64 text)
{
    if (label) {
        label->setText(QString::number(text));
    }
}
