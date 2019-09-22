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
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKBase.hh"
#include "SAKHidDeviceController.hh"
#include "ui_SAKHidDeviceController.h"

extern "C" {
#include "HidApi.h"
}

SAKHidDeviceController::SAKHidDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKHidDeviceController)
{
    ui->setupUi(this);

    hidDeviceComboBox = ui->hidDeviceComboBox;
    noMouseCheckBox = ui->noMouseCheckBox;
    noKeyboardCheckBox = ui->noKeyboardCheckBox;

    int ret = hid_init();
    Q_ASSERT_X(!ret, __FUNCTION__, "hid init failed");

    refresh();
}

SAKHidDeviceController::~SAKHidDeviceController()
{
    delete ui;
}

QString SAKHidDeviceController::localHost()
{
    return "";
}

quint16 SAKHidDeviceController::localPort()
{
    return 0;
}

QString SAKHidDeviceController::targetHost()
{
    return "";
}

quint16 SAKHidDeviceController::targetPort()
{
    return 0;
}

bool SAKHidDeviceController::enableCustomLocalSetting()
{
    return false;
}

void SAKHidDeviceController::refresh()
{
    initHidDeviceComboBox(hidDeviceComboBox, ignoreKeyWords());
}

void SAKHidDeviceController::setUiEnable(bool enable)
{
    hidDeviceComboBox->setEnabled(enable);
    noKeyboardCheckBox->setEnabled(enable);
    noMouseCheckBox->setEnabled(enable);
}

QString SAKHidDeviceController::devicePath()
{
    return hidDeviceComboBox->currentData().toString();
}

void SAKHidDeviceController::initHidDeviceComboBox(QComboBox *comboBox, QStringList ignoreKeyWord)
{
    if (comboBox){
        comboBox->clear();
        struct hid_device_info *hidInfo = hid_enumerate(0, 0);
        struct hid_device_info *hidInfoTemp = hidInfo;
        do {
            struct hid_device_info *device = hidInfo;
            if (device){
                bool ignore = false;
                QString item = QString::fromWCharArray(device->product_string);
                QString path = QString(device->path);
                for (auto var:ignoreKeyWord){
                    if (item.contains(var)){
                        ignore = true;
                        break;
                    }
                }

                if (!ignore){
                    comboBox->addItem(item, QVariant::fromValue(path));
                }
            }

            hidInfo = hidInfo->next;
        }while(hidInfo->next);
        hid_free_enumeration(hidInfoTemp);
    }
}

void SAKHidDeviceController::on_noMouseCheckBox_clicked()
{
    refresh();
}

void SAKHidDeviceController::on_noKeyboardCheckBox_clicked()
{
    refresh();
}

QStringList SAKHidDeviceController::ignoreKeyWords()
{
    QStringList keyWords;
    QString keyword;

    if (noKeyboardCheckBox->isChecked()){
        keyword =  QString("keyboard");
        keyWords.append(keyword);
        keyWords.append(keyword.toUpper());
    }

    if (noMouseCheckBox->isChecked()){
        keyword = QString("mouse");
        keyWords.append(keyword);
        keyWords.append(keyword.toUpper());
    }

    return keyWords;
}
