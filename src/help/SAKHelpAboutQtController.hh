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
#ifndef SAKHELPABOUTQTCONTROLLER_HH
#define SAKHELPABOUTQTCONTROLLER_HH

#include <QObject>

class SAKHelpManager;
class SAKHelpAboutQtController : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString officialRelease READ officialRelease CONSTANT)
    Q_PROPERTY(QString officialReleaseWiki READ officialReleaseWiki CONSTANT)
    Q_PROPERTY(QString officialBlog READ officialBlog CONSTANT)
    Q_PROPERTY(QString officialWeb READ officialWeb CONSTANT)
    Q_PROPERTY(QString chineseRelease READ chineseRelease CONSTANT)
    Q_PROPERTY(QString qtVersion READ qtVersion CONSTANT)
private:
    SAKHelpAboutQtController(QObject *parent = Q_NULLPTR);
    ~SAKHelpAboutQtController();
public:
    friend class SAKHelpManager;

    /**
     * @brief openUrl 打开链接
     * @param url 链接地址
     */
    Q_INVOKABLE void openUrl(QString url);
private:
    QString officialRelease();
    QString officialReleaseWiki();
    QString officialBlog();
    QString officialWeb();
    QString chineseRelease();
    QString qtVersion();
};

#endif
