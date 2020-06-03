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
    Ui::SAKHidDeviceController *ui;

    QComboBox *hidDeviceComboBox;
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
