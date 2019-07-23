/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#ifndef SAKOTHERSETTINGS_HH
#define SAKOTHERSETTINGS_HH

#include <QObject>
#include <QPushButton>

class TransmissionSettings;

class SAKDebugPage;

class SAKOtherSettings:public QObject
{
    Q_OBJECT
public:
    SAKOtherSettings(SAKDebugPage *debugPage, QObject *parent = nullptr);
    // 其他设置组
private:    
    QPushButton             *autoResponseSettingPushButton  = nullptr;  // 自动回复设置面板调出按钮
    QPushButton             *highlightSettingPushButton     = nullptr;  // 高亮设置面板调出按钮
    QPushButton             *readWriteSettingPushButton     = nullptr;  // 读写设置面板调出按钮
    QPushButton             *transmissionSettingPushButton  = nullptr;  // 数据转发设置面板调出按钮
    TransmissionSettings    *transmissionSettings;                      // 数据转发面板
    SAKDebugPage            *_debugPage                     = nullptr;  // 调试页面
private slots:
    void onAutoresponseSettingPushbuttonClicked();
    void onHighlightSettingPushButtonClicked();
    void onReadWriteSettingPushButtonClicked();
    void oTransmissionSettingPushButtonClicked();
};

#endif
