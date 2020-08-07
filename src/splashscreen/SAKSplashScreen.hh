/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
