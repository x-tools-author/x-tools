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

#include "SAKUsbDeviceController.hh"
#include "ui_SAKUsbDeviceController.h"

extern "C" {
#include "hidapi.h"
}

SAKUsbDeviceController::SAKUsbDeviceController(QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKUsbDeviceController)
{
    ui->setupUi(this);

    usbDeviceComboBox = ui->hidDeviceComboBox;
    noMouseCheckBox = ui->noMouseCheckBox;
    noKeyboardCheckBox = ui->noKeyboardCheckBox;
    endpointLineEdit = ui->endpointLineEdit;
    cmdLineEdit = ui->cmdLineEdit;

    noMouseCheckBox->setChecked(true);
    noKeyboardCheckBox->setChecked(true);

    int ret = hid_init();
    Q_ASSERT_X(!ret, __FUNCTION__, "Usb init failed");

    refresh();
}

SAKUsbDeviceController::~SAKUsbDeviceController()
{
    hid_exit();
    delete ui;
}

void SAKUsbDeviceController::refresh()
{
    initUsbDeviceComboBox(usbDeviceComboBox, ignoreKeyWords());
}

void SAKUsbDeviceController::setUiEnable(bool enable)
{
    usbDeviceComboBox->setEnabled(enable);
    noKeyboardCheckBox->setEnabled(enable);
    noMouseCheckBox->setEnabled(enable);
}

QString SAKUsbDeviceController::devicePath()
{
    uiMutex.lock();
    auto ret = usbDeviceComboBox->currentData().toString();
    uiMutex.unlock();
    return ret;
}

void SAKUsbDeviceController::initUsbDeviceComboBox(QComboBox *comboBox, QStringList ignoreKeyWord)
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

quint8 SAKUsbDeviceController::endpoint()
{
    uiMutex.lock();
    auto ret = static_cast<quint8>(endpointLineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

quint8 SAKUsbDeviceController::cmd()
{
    uiMutex.lock();
    auto ret = static_cast<quint8>(cmdLineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

void SAKUsbDeviceController::on_noMouseCheckBox_clicked()
{
    refresh();
}

void SAKUsbDeviceController::on_noKeyboardCheckBox_clicked()
{
    refresh();
}

QStringList SAKUsbDeviceController::ignoreKeyWords()
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
