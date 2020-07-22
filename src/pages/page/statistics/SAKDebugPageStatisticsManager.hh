/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom". It is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGPAGESTATISTICSMANAGER_HH
#define SAKDEBUGPAGESTATISTICSMANAGER_HH

#include <QTimer>
#include <QLabel>
#include <QObject>
#include <QPushButton>

class SAKDebugPage;
/// @brief statistics received bytes, frames and sent bytes, frames
class SAKDebugPageStatisticsManager:public QObject
{
    Q_OBJECT
public:
    SAKDebugPageStatisticsManager(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
private:
    SAKDebugPage *debugPage;

    QLabel *rxSpeedLabel;
    QLabel *txSpeedLabel;
    QLabel *txFramesLabel;
    QLabel *txBytesLabel;
    QLabel *rxFramesLabel;
    QLabel *rxBytesLabel;

    QPushButton *resetTxCountPushButton;
    QPushButton *resetRxCountPushButton;
private:
    /// @brief 数据统计项
    struct DataContext {
        quint64 txFrames;   // 发送帧数
        quint64 txBytes;    // 发送字节数
        quint64 rxFrames;   // 接收帧数
        quint64 rxBytes;    // 接收字节数
        quint64 rxBytesPerSecond;   // 每秒接收字节数
        quint64 txBytesPerSecond;   // 每秒发送字节数
    }dataContext;
    QTimer speedCalculationTimer;
private:
    void dataRead(QByteArray data);
    void dataReceived(QByteArray data);
    void clearRxStatistics();
    void clearTxStatistics();
    void setLabelText(QLabel *label, quint64 text);
    void speedCalculationTimerTimeout();
};

#endif
