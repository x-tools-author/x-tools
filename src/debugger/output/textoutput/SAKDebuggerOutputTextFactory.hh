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
class SAKDebuggerOutputSettings;
class SAKDebuggerTextOutput : public QObject
{
    Q_OBJECT
public:
    SAKDebuggerTextOutput(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerTextOutput();
private:
    SAKDebugger *debugger;
    SAKDebuggerOutputSettings *outputSettings;
private:
    /// @brief 输出信息，参数为设备读取到的数据
    void outputText(QByteArray text);
signals:
    void outputTextRequest(QString text);
};

#endif
