/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOUTPUTDATAFACTORY_HH
#define SAKOUTPUTDATAFACTORY_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "SAKDebugPageOutputManager.hh"

class SAKOutputDataFactory:public QThread
{
    Q_OBJECT
public:
    SAKOutputDataFactory(QObject *parent = Q_NULLPTR);
    ~SAKOutputDataFactory();

    /**
     * @brief cookData 将数据按照指定参数转变为字符串输出
     * @param rawData 原始数据（已接受数据或者已发送数据）
     * @param parameters 输出参数
     */
    void cookData(QByteArray rawData, SAKDebugPageOutputManager::OutputParameters parameters);
protected:
    void run() final;
private:
    struct RawDataStruct {
        QByteArray rawData;
        SAKDebugPageOutputManager::OutputParameters parameters;
    };

    QList<RawDataStruct> rawDataList;
    QMutex rawDataListMutex;
    QMutex threadMutex;
    QWaitCondition threadWaitCondition;
private:
    void innerCookData(QByteArray rawData, SAKDebugPageOutputManager::OutputParameters parameters);
    RawDataStruct takeRawData();
signals:
    void dataCooked(QString data);
};

#endif
