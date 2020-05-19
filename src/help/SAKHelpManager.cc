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
#include "SAKHelp3rdController.hh"
#include "SAKHelpBuyController.hh"
#include "SAKHelpHistoryController.hh"
#include "SAKHelpAboutQtController.hh"
#include "SAKHelpAboutSAKController.hh"
#include "SAKHelpDonationController.hh"

SAKHelpManager *SAKHelpManager::instancePtr = Q_NULLPTR;
SAKHelpManager::SAKHelpManager(QObject *parent)
    :QObject (parent)
{
    instancePtr = this;
    _trdController = new SAKHelp3rdController(this);
    _aboutQtController = new SAKHelpAboutQtController(this);
    _aboutSAKController = new SAKHelpAboutSAKController(this);
    _historyController = new SAKHelpHistoryController(this);
    _buyController = new SAKHelpBuyController(this);
    _donationController = new SAKHelpDonationController(this);
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

SAKHelp3rdController *SAKHelpManager::trdController()
{
    return _trdController;
}

SAKHelpAboutQtController *SAKHelpManager::aboutQtController()
{
    return _aboutQtController;
}

SAKHelpAboutSAKController *SAKHelpManager::aboutSAKController()
{
    return _aboutSAKController;
}

SAKHelpHistoryController *SAKHelpManager::historyController()
{
    return _historyController;
}

SAKHelpBuyController *SAKHelpManager::buyController()
{
    return _buyController;
}

SAKHelpDonationController *SAKHelpManager::donationController()
{
    return _donationController;
}
