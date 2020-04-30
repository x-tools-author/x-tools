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
#ifndef SAKAPPLICATION_HH
#define SAKAPPLICATION_HH

#include <QGuiApplication>
#include <QQmlApplicationEngine>

class SAKApplication:public QGuiApplication
{
    Q_OBJECT
public:
    SAKApplication(int argc, char **argv);
    ~SAKApplication();

    /**
     * @brief setupUi 加载ui界面
     */
    void setupUi();
private:
    QQmlApplicationEngine qmlAppEngine;
};

#endif
