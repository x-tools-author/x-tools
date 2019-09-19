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
#ifndef SAKHIDDEVICECONTROLLER_HH
#define SAKHIDDEVICECONTROLLER_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QSerialPort>

namespace Ui {
    class SAKHidDeviceController;
}

class SAKHidDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKHidDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKHidDeviceController();

    QString localHost();
    quint16 localPort();
    QString targetHost();
    quint16 targetPort();
    bool enableCustomLocalSetting();

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
    Ui::SAKHidDeviceController *ui;

    QComboBox * hidDeviceComboBox;
    QCheckBox *noMouseCheckBox;
    QCheckBox *noKeyboardCheckBox;
private slots:
    void on_noMouseCheckBox_clicked();
    void on_noKeyboardCheckBox_clicked();
private:
    QStringList ignoreKeyWords();
};

#endif
