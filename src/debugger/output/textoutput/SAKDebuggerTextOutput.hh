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
#ifndef SAKDEBUGGERTEXTOUTPUT_HH
#define SAKDEBUGGERTEXTOUTPUT_HH

#include <QObject>

class SAKDebugger;
class SAKDebuggerDevice;
class SAKDebuggerOutputSettings;
class SAKDebuggerOutputTextFactory;
class SAKDebuggerTextOutput : public QObject
{
    Q_OBJECT
public:
    SAKDebuggerTextOutput(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerTextOutput();

    /**
     * @brief textFactoryInstance 获取数据处理类实例指针
     * @return 数据处理类实例指针
     */
    SAKDebuggerOutputTextFactory *textFactoryInstance();

    /**
     * @brief setDevice 关联与设备相关的操作
     * @param device 设备实例指针
     */
    void setDevice(SAKDebuggerDevice *device);

    /**
     * @brief saveDataToFile 将输出数据保存至文件
     * @param data 带保存数据
     */
    Q_INVOKABLE void saveDataToFile(QString data);
private:
    SAKDebugger *debugger;
    SAKDebuggerOutputSettings *outputSettings;
    SAKDebuggerOutputTextFactory *textFactory;
private:
    /// @brief 输出信息，参数为设备读取到的数据
    void outputText(QByteArray text, bool isRxData);
    void outputTextRx(QByteArray text);
    void outputTextTx(QByteArray text);
   /// @brief 槽函数，接收factor处理后的数据
    void dataCooked(QString text);
signals:
    void outputTextRequest(QString text);
};

#endif
