/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QList>
#include <QDebug>
#include <QMetaEnum>
#include <QLineEdit>
#include <QSerialPortInfo>

#include "SAKHidDeviceController.hh"
#include "ui_SAKHidDeviceController.h"

extern "C" {
#include "hidapi.h"
}

SAKHidDeviceController::SAKHidDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKHidDeviceController)
{
    ui->setupUi(this);

    hidDeviceComboBox = ui->hidDeviceComboBox;
    noMouseCheckBox = ui->noMouseCheckBox;
    noKeyboardCheckBox = ui->noKeyboardCheckBox;

    noMouseCheckBox->setChecked(true);
    noKeyboardCheckBox->setChecked(true);

    int ret = hid_init();
    Q_ASSERT_X(!ret, __FUNCTION__, "hid init failed");

    refresh();
}

SAKHidDeviceController::~SAKHidDeviceController()
{
    hid_exit();
    delete ui;
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
    uiMutex.lock();
    auto ret = hidDeviceComboBox->currentData().toString();
    uiMutex.unlock();
    return ret;
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
                    if (item.toUpper().contains(var)){
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
