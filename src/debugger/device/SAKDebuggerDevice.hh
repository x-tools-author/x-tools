/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEBUGGERDEVICE_HH
#define SAKDEBUGGERDEVICE_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

class SAKDebugger;
class SAKDebuggerDevice : public QThread
{
    Q_OBJECT
    Q_PROPERTY(bool deviceIsOpened READ deviceIsOpened WRITE setDeviceIsOpened NOTIFY deviceIsOpenedChanged)
public:
    SAKDebuggerDevice(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerDevice();
public:
    /**
     * @brief open 打开设备，必须在子类中重新实现
     */
    virtual void open(){};
    /**
     * @brief refres 刷新设备，必须在子类中重新实现
     */
    virtual void refresh(){};

    /**
     * @brief wakeMe 唤醒线程
     */
    void wakeMe();
protected:
    bool openRequest;
    bool clostRequest;
    quint64 readInterval;
    SAKDebugger *debugger;
    QMutex deviceMutex;
    QWaitCondition deviceWaitCondition;
private:
    QMutex dataListMutex;
    /// @brief 待发送数据,子类定时检测该列表，存在数据则发送
    QList<QByteArray> dataList;
protected:
    /// @brief 将待发送数据添加值待发送数据列表中
    void writeBytes(QByteArray bytes);
    /// @brief 从待发送数据列表中提取数据
    QByteArray takeArrayFromDataList();
signals:
    /// @brief 读取到数据后出发该信号
    void bytesRead(QByteArray bytes);
    /// @brief 数据成功写入后出发该信号
    void bytesWritten(QByteArray bytes);
    /// @brief 设备状态发生改变时触发该信号
    void deviceStateChanged(bool opened);
private:
    bool _deviceIsOpened;
    bool deviceIsOpened();
    void setDeviceIsOpened(bool opened);
signals:
    void deviceIsOpenedChanged();
};

#endif
