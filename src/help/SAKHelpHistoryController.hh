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
#ifndef SAKHELPHISTORYCONTROLLER_HH
#define SAKHELPHISTORYCONTROLLER_HH

#include <QObject>

class SAKHelpManager;
class SAKHelpHistoryController : public QObject
{
    Q_OBJECT
private:
    SAKHelpHistoryController(QObject *parent = Q_NULLPTR);
    ~SAKHelpHistoryController();
public:
    friend class SAKHelpManager;

    /**
     * @brief readinHistoryInfo 读入发布历史信息
     * @return 历史信息描述
     */
    Q_INVOKABLE QString readinHistoryInfo();
};

#endif
