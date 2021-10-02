/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERSTATISTICS_H
#define SAKDEBUGGERSTATISTICS_H

#include <QLabel>
#include <QObject>
#include <QPushButton>

class SAKDebuggerStatistics:public QObject
{
    Q_OBJECT
public:
    SAKDebuggerStatistics(QLabel *txSpeed, QLabel *rxSpeed,
                          QLabel *txFrame,
                          QLabel *rxFrame,
                          QLabel *txBytes,
                          QLabel *rxBytes,
                          QObject *parent = Q_NULLPTR);
    void onBytesRead(QByteArray data);
    void onBytesWritten(QByteArray data);
private:
    QLabel *mTxSpeedLabel;
    QLabel *mRxSpeedLabel;
    QLabel *mTxFramesLabel;
    QLabel *mRxFramesLabel;
    QLabel *mTxBytesLabel;
    QLabel *mRxBytesLabel;

    struct SpinQStructStatisticsParametersContext {
        quint64 txFrames;   // The written frames
        quint64 txBytes;    // The written bytes
        quint64 rxFrames;   // The rread frames
        quint64 rxBytes;    // The read bytes
        quint64 rxSpeed;    // The bytes written in the passed one second
        quint64 txSpeed;    // The bytes read in the passed one second
    }mParametersContext;
private:
    void clearRxStatistics();
    void clearTxStatistics();
    void updateSpeed();
    void setLabelText(QLabel *label, quint64 text);
};

#endif
