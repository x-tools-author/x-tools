/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSPLASHSCREEN_HH
#define SAKSPLASHSCREEN_HH

#include <QSplashScreen>

/// @brief 启动界面
class SAKSplashScreen : public QSplashScreen
{
    Q_OBJECT
private:
    SAKSplashScreen();
    ~SAKSplashScreen();
public:
    static SAKSplashScreen *instance();

    /**
     * @brief showMessage 在启动界面上面显示信息
     * @param msg 待显示信息
     */
    void setMessage(QString msg);
private:
    static SAKSplashScreen *instancePtr;
};

#endif
