/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
