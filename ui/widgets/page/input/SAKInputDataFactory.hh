/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKINPUTDATAFACTORY_HH
#define SAKINPUTDATAFACTORY_HH

#include <QThread>

#include "SAKDebugPageInputManager.hh"

class SAKCRCInterface;
class SAKInputDataFactory:public QThread
{
    Q_OBJECT
public:
    SAKInputDataFactory(QObject *parent = Q_NULLPTR);

    /**
     * @brief cookData 处理输入数据
     * @param rawData 原数据
     * @param parameters 输入参数
     */
    void  cookData(QString rawData, SAKDebugPageInputManager::InputParameters parameters);

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
private:
    SAKCRCInterface *crcInterface;
    SAKDebugPage *debugPage;
    // ------------------------------------------------------------------------
    void run() final;      
signals:
    /// 输入数据经过处理后通过该信号对外发射
    void dataCooked(QByteArray);
};

#endif
