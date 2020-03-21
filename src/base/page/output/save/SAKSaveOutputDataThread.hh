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
#ifndef SAKSAVEOUTPUTDATATHREAD_HH
#define SAKSAVEOUTPUTDATATHREAD_HH

#include <QThread>

#include "SAKSaveOutputDataSettings.hh"

class SAKSaveOutputDataThread:public QThread
{
    Q_OBJECT
public:
    SAKSaveOutputDataThread(QObject *parent = nullptr);

    void writeDataToFile(QByteArray data, SAKSaveOutputDataSettings::SaveOutputDataParamters parameters);
private:
    void run() final;
};

#endif
