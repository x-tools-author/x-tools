/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKINPUTDATAFACTORY_HH
#define SAKINPUTDATAFACTORY_HH

#include <QMutex>
#include <QThread>
#include <QWaitCondition>

#include "SAKDebugPageInputManager.hh"

class SAKCRCInterface;
/// @brief 输入数据处理类
class SAKInputDataFactory:public QThread
{
    Q_OBJECT
public:
    SAKInputDataFactory(QObject *parent = Q_NULLPTR);
    ~SAKInputDataFactory();

    /**
     * @brief cookData 处理输入数据
     * @param rawData 原数据
     * @param parameters 输入参数
     */
    void cookData(QString rawData, SAKDebugPageInputManager::InputParameters parameters);

    /**
     * @brief crcCalculate 计算输入数据的crc
     * @param data 输入数据
     * @param model crc参数模型
     * @return crc
     */
    quint32 crcCalculate(QByteArray data, int model);

    /**
     * @brief rawDataToArray 将输入框的数据转换为数组
     * @param rawData 输入框数据
     * @param parameters 输入参数
     * @return 数据
     */
    QByteArray rawDataToArray(QString rawData, SAKDebugPageInputManager::InputParameters parameters);
protected:
    void run() final;
private:
    struct RawDataStruct {
        QString rawData;
        SAKDebugPageInputManager::InputParameters parameters;
    };

    QList<RawDataStruct> rawDataList;
    QMutex rawDataListMutex;
    QMutex threadMutex;
    QWaitCondition threadCondition;
    SAKCRCInterface *crcInterface;
    SAKDebugPage *debugPage;
private:
    RawDataStruct takeRawData();
    void innnerCookData(QString rawData, SAKDebugPageInputManager::InputParameters parameters);
signals:
    /// @brief 输入数据经过处理后通过该信号对外发射
    void dataCooked(QByteArray);
};

#endif
