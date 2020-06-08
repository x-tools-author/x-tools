/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKHIDDEVICECONTROLLER_HH
#define SAKHIDDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKHidDeviceController;
}

class SAKHidDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKHidDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKHidDeviceController();

    void refresh();
    void setUiEnable(bool enable);
    QString devicePath();

    /**
     * @brief initHidDeviceComboBox 初始化hid设备
     * @param comboBox 需要初始化的控件
     * @param ignoreKeyWord 关键字列表，根据关键字（如： MOUSE、KEYBOARD等）忽略某些hid设备
     */
    static void initHidDeviceComboBox(QComboBox *comboBox, QStringList ignoreKeyWord = QStringList());
private:
    QMutex uiMutex;
private:
    Ui::SAKHidDeviceController *ui;

    QComboBox *hidDeviceComboBox;
    QCheckBox *noMouseCheckBox;
    QCheckBox *noKeyboardCheckBox;
private slots:
    void on_noMouseCheckBox_clicked();
    void on_noKeyboardCheckBox_clicked();
private:
    QStringList ignoreKeyWords();
};

#endif
