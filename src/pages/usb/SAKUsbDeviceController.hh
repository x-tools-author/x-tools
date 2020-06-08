/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKUSBDEVICECONTROLLER_HH
#define SAKUSBDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKUsbDeviceController;
}

class SAKUsbDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKUsbDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKUsbDeviceController();

    void refresh();
    void setUiEnable(bool enable);
    QString devicePath();

    /**
     * @brief initUsbDeviceComboBox 初始化Usb设备
     * @param comboBox 需要初始化的控件
     * @param ignoreKeyWord 关键字列表，根据关键字（如： MOUSE、KEYBOARD等）忽略某些Usb设备
     */
    static void initUsbDeviceComboBox(QComboBox *comboBox, QStringList ignoreKeyWord = QStringList());

    /**
     * @brief endpoint 获取末端号
     * @return 末端编号
     */
    quint8 endpoint();

    /**
     * @brief cmd 获取命令编号
     * @return 命令编号
     */
    quint8 cmd();
private:
    QMutex uiMutex;
private:
    Ui::SAKUsbDeviceController *ui;

    QComboBox *usbDeviceComboBox;
    QCheckBox *noMouseCheckBox;
    QCheckBox *noKeyboardCheckBox;
    QLineEdit *endpointLineEdit;
    QLineEdit *cmdLineEdit;
private slots:
    void on_noMouseCheckBox_clicked();
    void on_noKeyboardCheckBox_clicked();
private:
    QStringList ignoreKeyWords();
};

#endif
