/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
