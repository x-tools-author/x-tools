/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSINGLETONCONTROLLER_HH
#define SAKSINGLETONCONTROLLER_HH

#include <QObject>
#include <QLocalServer>
#include <QSharedMemory>

/// @brief 单实例控制类
class SAKSingletonController:public QObject
{
    Q_OBJECT
public:
    SAKSingletonController(QObject *parent = Q_NULLPTR);
    ~SAKSingletonController();

    /**
     * @brief isInstanceExist 检测是否存在已运行的程序实例
     * @return true：已有程序实例在运行，false：不存在程序实例在运行
     */
    bool isInstanceExist();

    /**
     * @brief setFlag 设置启动标志
     */
    void setFlag();
private:
    QLocalServer *mLocalServer;
    QSharedMemory *mSharedMemory;
    bool mIsInstanceExist;
private:
    void checkSharedMemory();
signals:
    void showMainWindowInstanceRequest();
};

#endif
