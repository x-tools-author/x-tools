/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKAPPLICATIONINFORMATION_HH
#define SAKAPPLICATIONINFORMATION_HH

#include <QObject>

class SAKApplicationInformation:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief instance 获取app信息示例
     * @return App信息示例
     */
    static SAKApplicationInformation* instance();

    QString email();
    QString qqGroupNumber();
    QString authorName();
    QString authorNickname();
    QString version();
    QString buildTime();
    QString officeUrl();
    QString qqNumber();
    QString copyright();
    QString business();
private:
    SAKApplicationInformation(QObject *parent = nullptr);
    ~SAKApplicationInformation();

    static SAKApplicationInformation *thisInstance;
};

#endif
