/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKOUTPUTDATAFACTORY_HH
#define SAKOUTPUTDATAFACTORY_HH

#include <QThread>
#include "SAKDebugPageOutputManager.hh"

class SAKOutputDataFactory:public QThread
{
    Q_OBJECT
public:
    SAKOutputDataFactory(QObject *parent = nullptr);

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
