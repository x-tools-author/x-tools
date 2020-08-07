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

/// @brief A splash screen
class SAKSplashScreen : public QSplashScreen
{
    Q_OBJECT
private:
    SAKSplashScreen();
    ~SAKSplashScreen();
public:
    /**
     * @brief instance: Get the instance of splash screen
     * @return: Instance of splash screen
     */
    static SAKSplashScreen *instance();

    /**
     * @brief setMessage: Show message on the splash screen
     * @param msg: The message need to be show
     */
    void setMessage(QString msg);
private:
    static SAKSplashScreen *instancePtr;
};

#endif
