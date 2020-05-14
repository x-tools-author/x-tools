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
#ifndef SAKDEBUGGEROUTPUTMANAGER_HH
#define SAKDEBUGGEROUTPUTMANAGER_HH

#include <QObject>

class SAKDebugger;
class SAKDebuggerTextOutput;
class SAKDebuggerOutputSettings;
class SAKDebuggerOutputManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SAKDebuggerOutputSettings* outputSettings READ outputSettings CONSTANT)
    Q_PROPERTY(SAKDebuggerTextOutput* textOutput READ textOutput CONSTANT)
public:
    SAKDebuggerOutputManager(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerOutputManager();

    /**
     * @brief outputSettingsInstance 获取输出设置类实例指针
     * @return 输出设置类实例指针
     */
    SAKDebuggerOutputSettings *outputSettingsInstance();

    /**
     * @brief textOutputInstance 获取文本输出类实例指针
     * @return 文本输出类实例指针
     */
    SAKDebuggerTextOutput *textOutputInstance();
private:
    SAKDebugger *debugger;
private:
    SAKDebuggerOutputSettings *_outputSettings;
    SAKDebuggerOutputSettings *outputSettings();

    SAKDebuggerTextOutput* _textOutput;
    SAKDebuggerTextOutput* textOutput();
};

#endif
