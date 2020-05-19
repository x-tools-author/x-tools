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
#ifndef SAKHELP3RDCONTROLLER_HH
#define SAKHELP3RDCONTROLLER_HH

#include <QObject>

class SAKHelpManager;
class SAKHelp3rdController : public QObject
{
    Q_OBJECT
private:
    SAKHelp3rdController(QObject *parent = Q_NULLPTR);
    ~SAKHelp3rdController();
public:
    friend class SAKHelpManager;

    /**
     * @brief openUrl 打开链接
     * @param url 链接地址
     */
    Q_INVOKABLE void openUrl(QString url);
};

#endif
