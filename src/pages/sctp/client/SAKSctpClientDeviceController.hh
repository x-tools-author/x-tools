/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKSCTPCLIENTDEVICECONTROLLER_HH
#define SAKSCTPCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKSctpClientDeviceController;
}

class SAKSctpClientDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKSctpClientDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKSctpClientDeviceController();

    QString localHost();
    quint16 localPort();
    QString serverHost();
    quint16 serverPort();
    bool enableCustomLocalSetting();

    void refresh();
    void setUiEnable(bool enable);
private:
    QMutex uiMutex;
private:
    Ui::SAKSctpClientDeviceController *ui;
    QComboBox *localhostComboBox;
    QLineEdit *localPortlineEdit;
    QCheckBox *enableLocalSettingCheckBox;
    QLineEdit *serverHostLineEdit;
    QLineEdit *serverPortLineEdit;
};

#endif
