/*
 * Copyright (C) 2018-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/qsak/QtSwissArmyKnife
 *     https://gitee.com/qsak/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKUDPDEVICECONTROLLER_HH
#define SAKUDPDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
    class SAKUdpDeviceController;
}

class SAKUdpDevice;
class SAKUdpAdvanceSettingWidget;
class SAKUdpDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKUdpDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKUdpDeviceController();

    QString localHost();
    quint16 localPort();
    QString targetHost();
    quint16 targetPort();
    bool enableCustomLocalSetting();

    void refresh();
    void setUiEnable(bool enable);
    void setUdpDevice(SAKUdpDevice* device);
private:
    QMutex uiMutex;
private:
    Ui::SAKUdpDeviceController *ui;
    QComboBox *localhostComboBox;
    QLineEdit *localPortlineEdit;
    QCheckBox *enableLocalSettingCheckBox;
    QLineEdit *targetHostLineEdit;
    QLineEdit *targetPortLineEdit;
    QPushButton* advanceUdpPushButton;
private slots:
    void on_advanceUdpPushButton_clicked();
private:
    SAKUdpAdvanceSettingWidget* udpAdvanceSettingWidget;
};

#endif
