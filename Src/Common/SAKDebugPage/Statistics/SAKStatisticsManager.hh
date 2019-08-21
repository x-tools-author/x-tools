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
#ifndef SAKSTATISTICSMANAGER_HH
#define SAKSTATISTICSMANAGER_HH

#include <QLabel>
#include <QObject>
#include <QPushButton>

#include "SAKDebugPage.hh"

class SAKStatisticsManager:public QObject
{
    Q_OBJECT
public:
    SAKStatisticsManager(SAKDebugPage *debugPage, QObject *parent = nullptr);
private:
    SAKDebugPage *_debugPage;

    QLabel *txFramesLabel;
    QLabel *txBytesLabel;
    QLabel *rxFramesLabel;
    QLabel *rxBytesLabel;

    QPushButton *resetTxCountPushButton;
    QPushButton *resetRxCountPushButton;
private:
    /**
     * @brief 数据统计项
     */
    struct DataContext {
        quint64 txFrames;   // 发送帧数
        quint64 txBytes;    // 发送字节数
        quint64 rxFrames;   // 接收帧数
        quint64 rxBytes;    // 接收字节数
    }dataContext;
private:
    void dataReadOrwritten(QByteArray data, SAKDebugPage::OutputParameters parameters);
    void clearRxStatistics();
    void clearTxStatistics();
    void setLabelText(QLabel *label, quint64 text);
};

#endif
