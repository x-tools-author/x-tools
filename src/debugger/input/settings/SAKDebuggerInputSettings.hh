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
#ifndef SAKDEBUGGERINPURTSETTINGS_HH
#define SAKDEBUGGERINPURTSETTINGS_HH

#include <QTimer>
#include <QObject>

class SAKDebugger;
class SAKDebuggerInputSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList crcParameterModel READ crcParameterModel CONSTANT)
    Q_PROPERTY(QStringList textFormats READ textFormats CONSTANT)
    Q_PROPERTY(QString cyclicTime READ cyclicTime WRITE setCyclicTime NOTIFY cyclicTimeChanged)
public:
    enum InputTextFormat {
        Bin,
        Otc,
        Dec,
        Hex,
        Ascii,
        System
    };
    Q_ENUM(InputTextFormat)

    SAKDebuggerInputSettings(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInputSettings();

    /**
     * @brief avalidCRCParameterModel 获取调试器支持的crc参数模型
     * @return crc参数模型列表
     */
    static QStringList avalidCRCParameterModel();

    /**
     * @brief startTimer 启动循环发送定时器
     * @brief startRequest true标识启动定时器，false表示停止定时器
     */
    Q_INVOKABLE void startTimer(bool startRequest);

    /**
     * @brief writeBytes 发射发送请求信号
     */
    Q_INVOKABLE void writeBytes();
private:
    SAKDebugger *debugger;
    QTimer cyclicTimer;
private:
    QStringList crcParameterModel();
    QStringList textFormats();
signals:
    /// @brief 定时器溢出时发射该信号
    void writeBytesRequest();
private:
    quint32 _cyclicTime;
    QString cyclicTime();
    void setCyclicTime(QString ct);
signals:
    void cyclicTimeChanged();
};

#endif
