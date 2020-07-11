/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKSplashScreen.hh"

SAKSplashScreen *SAKSplashScreen::instancePtr = Q_NULLPTR;
SAKSplashScreen::SAKSplashScreen()
    :QSplashScreen(QPixmap(":/resources/images/StartUi.jpg"))
{
    instancePtr = this;
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setMessage(tr("瑞士军刀正在启动..."));
    setAttribute(Qt::WA_DeleteOnClose, true);
}

SAKSplashScreen::~SAKSplashScreen()
{
    instancePtr = Q_NULLPTR;
}

SAKSplashScreen* SAKSplashScreen::instance()
{
    if (!instancePtr){
        new SAKSplashScreen;
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "A null pointer!");
    return instancePtr;
}

void SAKSplashScreen::setMessage(QString msg)
{
    showMessage(msg, Qt::AlignBottom, QColor(255, 255, 255));
}
