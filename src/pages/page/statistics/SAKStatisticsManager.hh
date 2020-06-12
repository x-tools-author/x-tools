/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSTATISTICSMANAGER_HH
#define SAKSTATISTICSMANAGER_HH

#include <QTimer>
#include <QLabel>
#include <QObject>
#include <QPushButton>

class SAKDebugPage;
/// @brief 数据统计
class SAKStatisticsManager:public QObject
{
    Q_OBJECT
public:
    SAKStatisticsManager(SAKDebugPage *debugPage, QObject *parent = Q_NULLPTR);
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
