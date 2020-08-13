/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTESTDEVICECONTROLLER_HH
#define SAKTESTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKTestDeviceController;
}

class SAKTestDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKTestDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKTestDeviceController();

    void refresh(){};
private:
    Ui::SAKTestDeviceController *mUi;
};

#endif
