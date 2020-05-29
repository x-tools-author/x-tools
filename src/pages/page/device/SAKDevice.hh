/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEVICE_HH
#define SAKDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class SAKDebugPage;

/// @brief 设备抽象类
class SAKDevice:public QThread
{
    Q_OBJECT
public:
    SAKDevice(QObject *parent = Q_NULLPTR);
    ~SAKDevice();

    /**
     * @brief wakeMe 唤醒线程
     */
    void wakeMe();

    /**
     * @brief writeBytes 发送数据请求
     * @param bytes 待发送数据
     */
    void writeBytes(QByteArray bytes);
signals:
    /// @brief 数据发送成功后触发该信号
    void bytesWritten(QByteArray bytes);
    /// @brief 数据读取后触发该信号
    void bytesRead(QByteArray bytes);
    /// @brief 输出消息
    void messageChanged(QString msg, bool isInfo);
    /// @brief 设备开关状态发送改变是触发该信号
    void deviceStateChanged(bool isOpened);
protected:
    QMutex threadMutex;
    QWaitCondition threadWaitCondition;
    /*************************************************************************/
    /// @brief 提取待发送数据,无数据则返回空数据
    QByteArray takeWaitingForWrittingBytes();
private:
    QMutex waitingForWritingBytesListMutex;
    QList<QByteArray> waitingForWritingBytesList;
};

#endif
