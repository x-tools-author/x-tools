/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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
