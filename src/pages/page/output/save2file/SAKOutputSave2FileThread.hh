/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSAVEOUTPUTDATATHREAD_HH
#define SAKSAVEOUTPUTDATATHREAD_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "SAKOutputSave2FileDialog.hh"

class SAKOutputSave2FileThread:public QThread
{
    Q_OBJECT
public:
    SAKOutputSave2FileThread(QObject *parent = Q_NULLPTR);
    ~SAKOutputSave2FileThread();

    void writeDataToFile(QByteArray data, SAKOutputSave2FileDialog::SaveOutputDataParamters parameters);
protected:
    void run() final;
private:
    struct DataInfoStruct {
        QByteArray data;
        SAKOutputSave2FileDialog::SaveOutputDataParamters parameters;
    };

    QList<DataInfoStruct> dataList;
    QMutex dataListMutex;
    QMutex threadMutex;
    QWaitCondition threadWaitCondition;
private:
    void innerWriteDataToFile(QByteArray data, SAKOutputSave2FileDialog::SaveOutputDataParamters parameters);
    DataInfoStruct takeDataInfo();
};

#endif
