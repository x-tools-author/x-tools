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
#ifndef SAKDEBUGGERINPUTMANAGER_HH
#define SAKDEBUGGERINPUTMANAGER_HH

#include <QObject>

class SAKDebugger;
class SAKDebuggerTextInput;
class SAKDebuggerInputSettings;
class SAKDebuggerInputManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SAKDebuggerInputSettings* inputSettings READ inputSettings CONSTANT)
    Q_PROPERTY(SAKDebuggerTextInput* textInput READ textInput CONSTANT)
    Q_PROPERTY(QString currentString READ currentString WRITE setCurrentString NOTIFY currentStringChanged)
public:
    SAKDebuggerInputManager(SAKDebugger *debugger, QObject *parent = Q_NULLPTR);
    ~SAKDebuggerInputManager();

    /**
     * @brief inputSettingsInstance 获取输入设置实例指针
     * @return 输入设置实例指针
     */
    SAKDebuggerInputSettings *inputSettingsInstance();

    /**
     * @brief textInputInstance 获取文本输入实例指针
     * @return 文本输入实例指针
     */
    SAKDebuggerTextInput *textInputInstance();
private:
    SAKDebugger *debugger;
private:
    SAKDebuggerInputSettings *_inputSettings;
    SAKDebuggerInputSettings *inputSettings();

    SAKDebuggerTextInput *_textInput;
    SAKDebuggerTextInput *textInput();

    QString _currentString;
    QString currentString();
    void setCurrentString(QString str);
signals:
    void currentStringChanged();
};

#endif
