/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKDEBUGGEROUTPUTSTATISTICS_HH
#define SAKDEBUGGEROUTPUTSTATISTICS_HH

#include <QTimer>
#include <QObject>


class SAKDebuggerDevice;
/// @brief 数据统计
class SAKDebuggerOutputStatistics:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString rxSpeed  READ rxSpeed  NOTIFY rxSpeedChanged)
    Q_PROPERTY(QString txSpeed  READ txSpeed  NOTIFY txSpeedChanged)
    Q_PROPERTY(QString txFrames READ txFrames NOTIFY txFramesChanged)
    Q_PROPERTY(QString txBytes  READ txBytes  NOTIFY txBytesChanged)
    Q_PROPERTY(QString rxFrames READ rxFrames NOTIFY rxFramesChanged)
    Q_PROPERTY(QString rxBytes  READ rxBytes  NOTIFY rxBytesChanged)
public:
    SAKDebuggerOutputStatistics(QObject *parent = Q_NULLPTR);

    /**
     * @brief clearRxStatistics 清空接收数据统计
     */
    Q_INVOKABLE void clearRxStatistics();

    /**
     * @brief clearTxStatistics 清空发送数据统计
     */
    Q_INVOKABLE void clearTxStatistics();

    /**
     * @brief setDevice 关联设备相关操作
     * @param device 设备实例
     */
    void setDevice(SAKDebuggerDevice *device);
private:
    /// @brief 统计选项
    struct DataContext {
        quint64 txFrames;           // 发送帧数
        quint64 txBytes;            // 发送字节数
        quint64 rxFrames;           // 接收帧数
        quint64 rxBytes;            // 接收字节数
        quint64 rxBytesPerSecond;   // 每秒接收字节数
        quint64 txBytesPerSecond;   // 每秒发送字节数
    }dataContext;

    /// @brief 接收速率统计
    QTimer speedCalculationTimer;
private:
    /// @brief 更新接收速率显示
    void updateSpeed();
    void bytesRead(QByteArray bytes);
    void bytesWritten(QByteArray bytes);
private:
    QString _rxSpeed;
    QString rxSpeed();

    QString _txSpeed;
    QString txSpeed();

    QString txFrames();
    QString txBytes();
    QString rxFrames();
    QString rxBytes();
signals:
    void rxSpeedChanged();
    void txSpeedChanged();
    void txFramesChanged();
    void txBytesChanged();
    void rxFramesChanged();
    void rxBytesChanged();
};

#endif
