/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAK_HH
#define SAK_HH

#include <QObject>

/// @brief The class provide some interfaces to get informaton about QSAK project.
class SAK:public QObject
{
    Q_OBJECT
public:
    /**
     * @brief instance: Get the instance of SAK class
     * @return Instance of SAK class
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
