/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
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
