/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGESTATISTICSCONTROLLER_HH
#define SAKDEBUGPAGESTATISTICSCONTROLLER_HH

#include <QLabel>
#include <QObject>
#include <QPushButton>

class SAKDebugPage;
/// @brief statistics received bytes, frames and sent bytes, frames
class SAKDebugPageStatisticsController:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageStatisticsController(SAKDebugPage *mDebugPage, QObject *parent = Q_NULLPTR);
private:
    SAKDebugPage *mDebugPage;
    QLabel *mRxSpeedLabel;
    QLabel *mTxSpeedLabel;
    QLabel *mTxFramesLabel;
    QLabel *mTxBytesLabel;
    QLabel *mRxFramesLabel;
    QLabel *mRxBytesLabel;
    QPushButton *mResetTxCountPushButton;
    QPushButton *mResetRxCountPushButton;

    struct ParametersContext {
        quint64 txFrames;   // written frames
        quint64 txBytes;    // written bytes
        quint64 rxFrames;   // read frames
        quint64 rxBytes;    // read bytes
        quint64 rxSpeed;    // bytes written in the passed one second
        quint64 txSpeed;    // bytes read in the passed one second
    }mParametersContext;
private:
    void bytesRead(QByteArray data);
    void bytesWritten(QByteArray data);
    void clearRxStatistics();
    void clearTxStatistics();
    void updateSpeed();
    void setLabelText(QLabel *label, quint64 text);
};

#endif
