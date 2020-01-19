/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com
 */
#ifndef SAKTCPCLIENTDEVICECONTROLLER_HH
#define SAKTCPCLIENTDEVICECONTROLLER_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKTcpClientDeviceController;
}

class SAKTcpClientDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKTcpClientDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKTcpClientDeviceController();

    QString localHost();
    quint16 localPort();
    QString serverHost();
    quint16 serverPort();
    bool enableCustomLocalSetting();

    void refresh();
    void setUiEnable(bool enable);
private:
    Ui::SAKTcpClientDeviceController *ui;

    QComboBox *localhostComboBox;
    QLineEdit *localPortlineEdit;
    QCheckBox *enableLocalSettingCheckBox;
    QLineEdit *serverHostLineEdit;
    QLineEdit *serverPortLineEdit;
};

#endif
