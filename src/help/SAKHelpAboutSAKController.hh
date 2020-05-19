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
#ifndef SAKHELPABOUTSAKCONTROLLER_HH
#define SAKHELPABOUTSAKCONTROLLER_HH

#include <QObject>

class SAKHelpManager;
class SAKHelpAboutSAKController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString authorName READ authorName CONSTANT)
    Q_PROPERTY(QString authorEmail READ authorEmail CONSTANT)
    Q_PROPERTY(QString buildDateTime READ buildDateTime CONSTANT)
    Q_PROPERTY(QString copyright READ copyright CONSTANT)

    Q_PROPERTY(QString githubUrl READ githubUrl CONSTANT)
    Q_PROPERTY(QString gitteeUrl READ gitteeUrl CONSTANT)
    Q_PROPERTY(QString officialRelease READ officialRelease CONSTANT)
private:
    SAKHelpAboutSAKController(QObject *parent = Q_NULLPTR);
    ~SAKHelpAboutSAKController();
public:
    friend class SAKHelpManager;

    /**
     * @brief openUrl 打开链接
     * @param url 链接地址
     */
    Q_INVOKABLE void openUrl(QString url);
private:
    QString appVersion();
    QString authorName();
    QString authorEmail();
    QString buildDateTime();
    QString copyright();

    QString githubUrl();
    QString gitteeUrl();
    QString officialRelease();
};

#endif
