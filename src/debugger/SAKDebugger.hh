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
#ifndef SAKDEBUGGER_HH
#define SAKDEBUGGER_HH

#include <QMutex>
#include <QObject>

class SAKCRCInterface;
class SAKDebuggerDevice;
class SAKDebuggerInputManager;
class SAKDebuggerOutputManager;
class SAKDebuggerDeviceSerialport;
class SAKDebugger : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SAKDebuggerDevice* device READ device NOTIFY deviceChanged)
    Q_PROPERTY(SAKDebuggerInputManager* inputManager READ inputManager CONSTANT)
    Q_PROPERTY(SAKDebuggerOutputManager* outputManager READ outputManager CONSTANT)
public:
    enum SAKDebuggertype {
        DebuggerTypeSerialport,
        DebuggerTypeUDP,
        DebuggerTypeTcpClient,
        DebuggerTypeTcpServer
    };
    Q_ENUM(SAKDebuggertype)

    SAKDebugger(int type, QObject *parent = Q_NULLPTR);
    ~SAKDebugger();

    /**
     * @brief setMessage 输出消息，可在不同线程中调用该接口，该接口是线程安全的
     * @param msg 消息类容
     * @param isError 为true是表示消息时错误消息
     */
    void setMessage(QString msg, bool isError = false);

    /**
     * @brief inputManagerInstance 获取输入管理类实例指针
     * @return 输入管理类实例指针
     */
    SAKDebuggerInputManager *inputManagerInstance();

    /**
     * @brief outputManagerInstance 获取输出管理类实例指针
     * @return 输出管理类实例指针
     */
    SAKDebuggerOutputManager *outputManagerInstance();

    /**
     * @brief deviceInstance 获取设备实例指针
     * @return 设备实例指针
     */
    SAKDebuggerDevice *deviceInstance();
private:
    int debuggerType;
    SAKCRCInterface *crcInterface;
    QMutex messageMutex;
private:
    SAKDebuggerDevice *_device;
    SAKDebuggerDevice *device();

    SAKDebuggerInputManager *_inputManager;
    SAKDebuggerInputManager *inputManager();

    SAKDebuggerOutputManager *_outputManager;
    SAKDebuggerOutputManager *outputManager();
signals:
    void deviceChanged();
};

#endif
