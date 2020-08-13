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

/// @brief Singleton application controller
class SAKSingletonController:public QObject
{
    Q_OBJECT
public:
    SAKSingletonController(QObject *parent = Q_NULLPTR);
    ~SAKSingletonController();

    /**
     * @brief isInstanceExist: true-SAK is running
     * @return the state of SAK
     */
    bool isInstanceExist();

    /**
     * @brief setFlag: Write shared memory
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
