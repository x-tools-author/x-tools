/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTOOLSMANAGER_HH
#define SAKTOOLSMANAGER_HH

#include <QMap>
#include <QObject>

/// @brief 实用工具管理类，该类是一个单实例类
class SAKToolsManager : public QObject
{
    Q_OBJECT
private:
    SAKToolsManager(QObject *parent = Q_NULLPTR);
    ~SAKToolsManager();
public:
    /**
     * @brief instance 获取唯一的实例指针
     * @return 实例指针
     */
    static SAKToolsManager *instance();

    /**
     * @brief showToolWidget 显示工具窗口
     */
    void showToolWidget(int type);
private:
    static SAKToolsManager *instancePtr;
    QMap<int, QWidget *> mToolsMap;
private:
    /// @brief 创建指定类型的工具
    QWidget *toolWidgetFromType(int type);
};

#endif
