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
#ifndef SAKDEBUGGERTEXTINPUT_HH
#define SAKDEBUGGERTEXTINPUT_HH

#include <QObject>

class SAKDebugger;
class SAKDebuggerInputSettings;
class SAKDebuggerTextInput : public QObject
{
    Q_OBJECT
public:
    SAKDebuggerTextInput(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerTextInput();

    /**
     * @brief wirteBytes 发送数据
     * @param bytes 待发送数据
     */
    Q_INVOKABLE void writeBytes(QByteArray bytes);

    /**
     * @brief writeRawData 写（发送）数据
     * @param data 待发送数据，格式为输入框文本格式
     */
    Q_INVOKABLE void writeRawData(QString data);
private:
    SAKDebugger *debugger;
signals:
    void writeBytesRequest(QByteArray bytes);
};

#endif
