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
#ifndef SAKHELPMANAGER_HH
#define SAKHELPMANAGER_HH

#include <QObject>

class SAKHelp3rdController;
class SAKHelpBuyController;
class SAKHelpAboutQtController;
class SAKHelpHistoryController;
class SAKHelpAboutSAKController;
class SAKHelpDonationController;
class SAKHelpManager : public QObject
{
    Q_OBJECT
    Q_PROPERTY(SAKHelp3rdController* trdController READ trdController CONSTANT)
    Q_PROPERTY(SAKHelpAboutQtController* aboutQtController READ aboutQtController CONSTANT)
    Q_PROPERTY(SAKHelpAboutSAKController* aboutSAKController READ aboutSAKController CONSTANT)
    Q_PROPERTY(SAKHelpHistoryController* historyController READ  historyController CONSTANT)
    Q_PROPERTY(SAKHelpBuyController* buyController READ buyController CONSTANT)
    Q_PROPERTY(SAKHelpDonationController* donationController READ donationController CONSTANT)
private:
    SAKHelpManager(QObject *parent = Q_NULLPTR);
    ~SAKHelpManager();
public:
    static SAKHelpManager *instance();
private:
    static SAKHelpManager *instancePtr;
private:
    SAKHelp3rdController *_trdController;
    SAKHelp3rdController *trdController();

    SAKHelpAboutQtController *_aboutQtController;
    SAKHelpAboutQtController *aboutQtController();

    SAKHelpAboutSAKController *_aboutSAKController;
    SAKHelpAboutSAKController *aboutSAKController();

    SAKHelpHistoryController *_historyController;
    SAKHelpHistoryController *historyController();

    SAKHelpBuyController *_buyController;
    SAKHelpBuyController *buyController();

    SAKHelpDonationController *_donationController;
    SAKHelpDonationController *donationController();
};

#endif
