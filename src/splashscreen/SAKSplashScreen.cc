/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKSplashScreen.hh"

SAKSplashScreen *SAKSplashScreen::instancePtr = Q_NULLPTR;
SAKSplashScreen::SAKSplashScreen()
    :QSplashScreen(QPixmap(":/resources/images/StartUi.jpg"))
{
    instancePtr = this;
    setWindowFlags(windowFlags() | Qt::WindowStaysOnTopHint);
    setMessage(tr("SAK is starting..."));
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
