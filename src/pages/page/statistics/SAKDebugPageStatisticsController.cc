/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QTimer>

#include "SAKDebugPage.hh"
#include "SAKDebugPageStatisticsController.hh"

SAKDebugPageStatisticsController::SAKDebugPageStatisticsController(SAKDebugPage *debugPage, QObject *parent)
    :QObject(parent)
    ,mDebugPage(debugPage)
{
    mParametersContext.rxBytes = 0;
    mParametersContext.txBytes = 0;
    mParametersContext.rxFrames = 0;
    mParametersContext.txFrames = 0;
    mParametersContext.rxSpeed = 0;
    mParametersContext.txSpeed = 0;

    Q_ASSERT_X(debugPage, __FUNCTION__, "The parameter(debugPage) can not be null");
    // if the parameter(debugPage) is null, the class will not take effect
    if (mDebugPage){
        mRxSpeedLabel = mDebugPage->mRxSpeedLabel;
        mTxSpeedLabel = mDebugPage->mTxSpeedLabel;
        mTxFramesLabel = mDebugPage->mTxFramesLabel;
        mRxFramesLabel = mDebugPage->mRxFramesLabel;
        mTxBytesLabel = mDebugPage->mTxBytesLabel;
        mRxBytesLabel = mDebugPage->mRxBytesLabel;
        mResetTxCountPushButton = mDebugPage->mResetTxCountPushButton;
        mResetRxCountPushButton = mDebugPage->mResetRxCountPushButton;

        // emmm...
        connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKDebugPageStatisticsController::bytesRead);
        connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKDebugPageStatisticsController::bytesWritten);
        connect(mResetRxCountPushButton, &QPushButton::clicked, this, &SAKDebugPageStatisticsController::clearRxStatistics);
        connect(mResetTxCountPushButton, &QPushButton::clicked, this, &SAKDebugPageStatisticsController::clearTxStatistics);

        // update speed
        QTimer *speedTimer = new QTimer(this);
        speedTimer->setInterval(1*1000);
        connect(speedTimer, &QTimer::timeout, this, &SAKDebugPageStatisticsController::updateSpeed);
        speedTimer->start();
    }
}

void SAKDebugPageStatisticsController::bytesRead(QByteArray data)
{
    mParametersContext.rxFrames += 1;
    mParametersContext.rxBytes += static_cast<quint64>(data.length());
    setLabelText(mRxFramesLabel, mParametersContext.rxFrames);
    setLabelText(mRxBytesLabel, mParametersContext.rxBytes);

    mParametersContext.rxSpeed += static_cast<quint64>(data.length());
}

void SAKDebugPageStatisticsController::bytesWritten(QByteArray data)
{
    mParametersContext.txFrames += 1;
    mParametersContext.txBytes += static_cast<quint64>(data.length());
    setLabelText(mTxFramesLabel, mParametersContext.txFrames);
    setLabelText(mTxBytesLabel, mParametersContext.txBytes);

    mParametersContext.txSpeed += static_cast<quint64>(data.length());
}

void SAKDebugPageStatisticsController::clearRxStatistics()
{
    mParametersContext.rxBytes = 0;
    mParametersContext.rxFrames = 0;
    setLabelText(mRxFramesLabel, mParametersContext.rxFrames);
    setLabelText(mRxBytesLabel, mParametersContext.rxBytes);
}

void SAKDebugPageStatisticsController::clearTxStatistics()
{
    mParametersContext.txBytes = 0;
    mParametersContext.txFrames = 0;
    setLabelText(mTxFramesLabel, mParametersContext.txFrames);
    setLabelText(mTxBytesLabel, mParametersContext.txBytes);
}

void SAKDebugPageStatisticsController::updateSpeed()
{
    auto toSpeedString = [](QLabel *label, quint64 &bytes){
        if (bytes < 1024){
            label->setText(QString("%1 B/s").arg(bytes));
        }else if(bytes < (1024*1024)){
            label->setText(QString("%1 KB/s").arg(bytes/1024));
        }else{
            label->setText(QString("%1 MB/s").arg(bytes/(1024*1024)));
        }

        bytes = 0;
    };

    toSpeedString(mRxSpeedLabel, mParametersContext.rxSpeed);
    toSpeedString(mTxSpeedLabel, mParametersContext.txSpeed);
}

void SAKDebugPageStatisticsController::setLabelText(QLabel *label, quint64 text)
{
    if (label){
        label->setText(QString::number(text));
    }
}
