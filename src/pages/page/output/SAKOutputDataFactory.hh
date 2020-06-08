/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKOUTPUTDATAFACTORY_HH
#define SAKOUTPUTDATAFACTORY_HH

#include <QThread>
#include "SAKDebugPageOutputManager.hh"

class SAKOutputDataFactory:public QThread
{
    Q_OBJECT
public:
    SAKOutputDataFactory(QObject *parent = Q_NULLPTR);

    /**
     * @brief cookData 将数据按照指定参数转变为字符串输出
     * @param rawData 原始数据（已接受数据或者已发送数据）
     * @param parameters 输出参数
     */
    void cookData(QByteArray rawData, SAKDebugPageOutputManager::OutputParameters parameters);
private:
    void run() final;
signals:
    void dataCooked(QString data);
};

#endif
