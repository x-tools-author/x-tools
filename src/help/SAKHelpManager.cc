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
#include <QCoreApplication>

#include "SAKHelpManager.hh"
#include "SAKHelpAboutQtController.hh"
#include "SAKHelpAboutSAKController.hh"

SAKHelpManager *SAKHelpManager::instancePtr = Q_NULLPTR;
SAKHelpManager::SAKHelpManager(QObject *parent)
    :QObject (parent)
{
    instancePtr = this;
    _aboutQtController = new SAKHelpAboutQtController(this);
    _aboutSAKController = new SAKHelpAboutSAKController(this);
}

SAKHelpManager::~SAKHelpManager()
{
    instancePtr = Q_NULLPTR;
}

SAKHelpManager *SAKHelpManager::instance()
{
    if (!instancePtr){
        new SAKHelpManager(qApp);
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Oh, a null pointer!");
    return instancePtr;
}

SAKHelpAboutQtController *SAKHelpManager::aboutQtController()
{
    return _aboutQtController;
}

SAKHelpAboutSAKController *SAKHelpManager::aboutSAKController()
{
    return _aboutSAKController;
}
