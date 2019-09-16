/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QList>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKBase.hh"
<<<<<<< HEAD
#include "SAKHidDeviceController.hh"
#include "ui_SAKHidDeviceController.h"
SAKHidDeviceController::SAKHidDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKHidDeviceController)
=======
#include "SAKUdpDeviceController.hh"
#include "ui_SAKUdpDeviceController.h"
SAKUdpDeviceController::SAKUdpDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUdpDeviceController)
>>>>>>> developer
{
    ui->setupUi(this);

    localhostComboBox = ui->localhostComboBox;
    localPortlineEdit = ui->localPortlineEdit;
    enableLocalSettingCheckBox = ui->enableLocalSettingCheckBox;
    targetHostLineEdit = ui->targetHostLineEdit;
    targetPortLineEdit = ui->targetPortLineEdit;

    refresh();
}

<<<<<<< HEAD
SAKHidDeviceController::~SAKHidDeviceController()
=======
SAKUdpDeviceController::~SAKUdpDeviceController()
>>>>>>> developer
{
    delete ui;
}

<<<<<<< HEAD
QString SAKHidDeviceController::localHost()
=======
QString SAKUdpDeviceController::localHost()
>>>>>>> developer
{
    return localhostComboBox->currentText();
}

<<<<<<< HEAD
quint16 SAKHidDeviceController::localPort()
=======
quint16 SAKUdpDeviceController::localPort()
>>>>>>> developer
{
    return static_cast<quint16>(localPortlineEdit->text().toInt());
}

<<<<<<< HEAD
QString SAKHidDeviceController::targetHost()
=======
QString SAKUdpDeviceController::targetHost()
>>>>>>> developer
{
    return targetHostLineEdit->text();
}

<<<<<<< HEAD
quint16 SAKHidDeviceController::targetPort()
=======
quint16 SAKUdpDeviceController::targetPort()
>>>>>>> developer
{
    return static_cast<quint16>(targetPortLineEdit->text().toInt());
}

<<<<<<< HEAD
bool SAKHidDeviceController::enableCustomLocalSetting()
=======
bool SAKUdpDeviceController::enableCustomLocalSetting()
>>>>>>> developer
{
    return enableLocalSettingCheckBox->isChecked();
}

<<<<<<< HEAD
void SAKHidDeviceController::refresh()
=======
void SAKUdpDeviceController::refresh()
>>>>>>> developer
{
    SAKBase::instance()->initIpComboBox(localhostComboBox);
}

<<<<<<< HEAD
void SAKHidDeviceController::setUiEnable(bool enable)
=======
void SAKUdpDeviceController::setUiEnable(bool enable)
>>>>>>> developer
{
    localhostComboBox->setEnabled(enable);
    localPortlineEdit->setEnabled(enable);
    enableLocalSettingCheckBox->setEnabled(enable);
    targetHostLineEdit->setEnabled(enable);
    targetPortLineEdit->setEnabled(enable);
}
