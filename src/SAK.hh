/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
