/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKUDPADCANCESETTINGS_HH
#define SAKUDPADCANCESETTINGS_HH

#include <QWidget>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>
#include <QListWidget>

namespace Ui {
    class SAKUdpAdvanceSettingsWidget;
}

class SAKUdpDevice;
class SAKUdpAdvanceSettingWidget:public QWidget
{
    Q_OBJECT
public:
    SAKUdpAdvanceSettingWidget(QWidget *parent = Q_NULLPTR);
    ~SAKUdpAdvanceSettingWidget();

    void setUdpDevice(SAKUdpDevice* device);
private:
    Ui::SAKUdpAdvanceSettingsWidget *ui;

    QCheckBox* unicastCheckBox;
    QCheckBox* broadcastCheckBox;
    QLineEdit* broadcastPortLineEdit;
    QCheckBox* multicastCheckBox;
    QListWidget* listWidget;
    QPushButton* deletePushButton;
    QPushButton* addPushButton;
private slots:
    void on_unicastCheckBox_clicked();
    void on_broadcastCheckBox_clicked();
    void on_multicastCheckBox_clicked();
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
private:
    SAKUdpDevice* udpDevice;
    bool isInitUi;
};

#endif
