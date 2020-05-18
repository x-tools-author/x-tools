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
#include <QMutex>
#include <QObject>

class SAKDebugger;
class SAKDebuggerInputSettings : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QStringList crcParameterModel READ crcParameterModel CONSTANT)
    Q_PROPERTY(QStringList textFormats READ textFormats CONSTANT)
    Q_PROPERTY(QString cyclicTime READ cyclicTime WRITE setCyclicTime NOTIFY cyclicTimeChanged)

    Q_PROPERTY(QString currentCRCModel READ currentCRCModel WRITE setCurrentCRCModel NOTIFY currentCRCModelChanged)
    Q_PROPERTY(QString currentInputFromat READ currentInputFromat WRITE setCurrentInputFromat NOTIFY currentInputFromatChanged)
public:
    enum InputTextFormat {
        Bin,
        Otc,
        Dec,
        Hex,
        Ascii,
        Utf8,
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

    /**
     * @brief paraCurrentCRCModel 获取当前crc参数模型
     * @return 当前crc参数模型
     */
    int paraCurrentCRCModel();

    /**
     * @brief paraCurrentInputFromat 获取当前输入格式
     * @return 当前输入格式
     */
    int paraCurrentInputFromat();
private:
    SAKDebugger *debugger;
    QTimer cyclicTimer;
private:
    QStringList crcParameterModel();
    QStringList textFormats();

    QMutex currentCRCModelMutex;
    QMutex currentInputFormatMutex;
signals:
    /// @brief 定时器溢出时发射该信号
    void writeBytesRequest();
private:
    quint32 _cyclicTime;
    QString cyclicTime();
    void setCyclicTime(QString ct);

    int _currentCRCModel;
    QString currentCRCModel();
    void setCurrentCRCModel(QString model);

    int _currentInputFormat;
    QString currentInputFromat();
    void setCurrentInputFromat(QString model);
signals:
    void cyclicTimeChanged();
    void currentCRCModelChanged();
    void currentInputFromatChanged();
};

#endif
