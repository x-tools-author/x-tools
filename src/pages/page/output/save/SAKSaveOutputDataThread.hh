/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSAVEOUTPUTDATATHREAD_HH
#define SAKSAVEOUTPUTDATATHREAD_HH

#include <QThread>

#include "SAKSaveOutputDataSettings.hh"

class SAKSaveOutputDataThread:public QThread
{
    Q_OBJECT
public:
    SAKSaveOutputDataThread(QObject *parent = Q_NULLPTR);

    void writeDataToFile(QByteArray data, SAKSaveOutputDataSettings::SaveOutputDataParamters parameters);
private:
    void run() final;
};

#endif
