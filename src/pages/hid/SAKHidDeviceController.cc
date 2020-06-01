/*
 * Copyright (C) 2019-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
    endpointLineEdit = ui->endpointLineEdit;
    cmdLineEdit = ui->cmdLineEdit;

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

quint8 SAKHidDeviceController::endpoint()
{
    uiMutex.lock();
    auto ret = static_cast<quint8>(endpointLineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
}

quint8 SAKHidDeviceController::cmd()
{
    uiMutex.lock();
    auto ret = static_cast<quint8>(cmdLineEdit->text().toInt());
    uiMutex.unlock();
    return ret;
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
