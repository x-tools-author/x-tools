/*
 * Copyright (C) 2019-2020 Qter. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKAPPLICATIONINFORMATION_HH
#define SAKAPPLICATIONINFORMATION_HH

#include <QObject>

class SAK:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief instance 获取app信息示例
     * @return App信息示例
     */
    static SAK* instance();

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
    SAK(QObject *parent = Q_NULLPTR);
    ~SAK();

    static SAK *thisInstance;
};

#endif
