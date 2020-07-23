/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKOUTPUTSAVE2FILETHREAD_HH
#define SAKOUTPUTSAVE2FILETHREAD_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "SAKOutputSave2FileDialog.hh"
/// @brief the task of the thread is that writting data to file
class SAKOutputSave2FileThread:public QThread
{
    Q_OBJECT
public:
    SAKOutputSave2FileThread(QObject *parent = Q_NULLPTR);
    ~SAKOutputSave2FileThread();

    void writeDataToFile(QByteArray data, SAKOutputSave2FileDialog::ParametersContext parameters);
protected:
    void run() final;
private:
    struct DataInfoStruct {
        QByteArray data;
        SAKOutputSave2FileDialog::ParametersContext parameters;
    };

    QList<DataInfoStruct> mDataList;
    QMutex mDataListMutex;
    QMutex mThreadMutex;
    QWaitCondition mThreadWaitCondition;
private:
    void innerWriteDataToFile(QByteArray data, SAKOutputSave2FileDialog::ParametersContext parameters);
    DataInfoStruct takeDataInfo();
    QString bytes2String(QByteArray bytes, int format);
};

#endif
