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
#ifndef SAKDEBUGGERDEVICESESERIALPORT_HH
#define SAKDEBUGGERDEVICESESERIALPORT_HH

#include <QSerialPort>
#include "SAKDebuggerDevice.hh"

class SAKDebugger;
class SAKDebuggerDeviceSerialport : public SAKDebuggerDevice
{
    Q_OBJECT
    Q_PROPERTY(QStringList serialports READ serialports NOTIFY serialportsChanged)
    Q_PROPERTY(QStringList baudRates READ baudRates CONSTANT)
    Q_PROPERTY(QStringList stopBits READ stopBits CONSTANT)
    Q_PROPERTY(QStringList paritys READ paritys CONSTANT)
    Q_PROPERTY(QStringList dataBits READ dataBits CONSTANT)

    /// @brief 以下是串口参数相关属性
    Q_PROPERTY(QString paraPortName READ paraPortName WRITE setParaPortName NOTIFY paraPortNameChanged)
    Q_PROPERTY(QString paraBaudRate READ paraBaudRate WRITE setParaBaudRate NOTIFY paraBaudRateChanged)
    Q_PROPERTY(QString paraDataBits READ paraDataBits WRITE setParaDataBits NOTIFY paraDataBitsChanged)
    Q_PROPERTY(QString paraStopBits READ paraStopBits WRITE setParaStopBits NOTIFY paraStopBitsChanged)
    Q_PROPERTY(QString paraParity READ paraParity WRITE setParaParity NOTIFY paraParityChanged)
public:
    SAKDebuggerDeviceSerialport(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerDeviceSerialport();

    /**
     * @brief avalidSerialports 获取系统可用串口设备列表
     * @return 系统串口设备列表
     */
    static QStringList avalidSerialports();

    /**
     * @brief avalidBaudRates 获取软件支持的波特率列表
     * @return 波特率列表
     */
    static QStringList avalidBaudRates();

    /**
     * @brief avalidStopBits 获取串口设备支持的停止位列表
     * @return 停止位列表
     */
    static QStringList avalidStopBits();

    /**
     * @brief avalidParitys 获取系统支持的奇偶校验为列表
     * @return 奇偶校验位列表
     */
    static QStringList avalidParitys();

    /**
     * @brief avalidDataBits 获取支持的数据位列表
     * @return 数据位列表
     */
    static QStringList avalidDataBits();

    /**
     * @brief open 打开设备
     */
    Q_INVOKABLE void open() final;

    /**
     * @brief refreshDevice 刷新设备列表
     */
    Q_INVOKABLE void refresh() final;
protected:
    void run() final;
private:
    /// @brief 串口参数上下文
    struct ParametersContext {
        QString portName;
        QString baudRate;
        QString dataBits;
        QString stopBits;
        QString parity;
    };

    QSerialPort *serialport;
    struct ParametersContext parameterContext;
private:
    /// @brief 打开串口设备
    bool openActually(QSerialPort *port);
private:
    QStringList _serialports;
    QStringList serialports();

    QStringList _baudRates;
    QStringList baudRates();

    QStringList _stopBits;
    QStringList stopBits();

    QStringList _paritys;
    QStringList paritys();

    QStringList _dataBits;
    QStringList dataBits();

    QString paraPortName();
    void setParaPortName(QString name);

    QString paraBaudRate();
    void setParaBaudRate(QString br);

    QString paraDataBits();
    void setParaDataBits(QString db);

    QString paraStopBits();
    void setParaStopBits(QString sb);

    QString paraParity();
    void setParaParity(QString p);
signals:
    void serialportsChanged();
    void paraPortNameChanged();
    void paraBaudRateChanged();
    void paraDataBitsChanged();
    void paraStopBitsChanged();
    void paraParityChanged();
};

#endif
