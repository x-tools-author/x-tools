/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
