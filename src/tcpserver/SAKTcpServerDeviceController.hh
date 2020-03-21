/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTCPSERVERDEVICECONTROLLER_HH
#define SAKTCPSERVERDEVICECONTROLLER_HH

#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKTcpServerDeviceController;
}

class SAKTcpServerDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKTcpServerDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerDeviceController();

    QString serverHost();
    quint16 serverPort();

    QString currentClientHost();
    quint16 currentClientPort();

    void refresh();
    void setUiEnable(bool enable);

    void addClient(QString host, quint16 port);
private:
    Ui::SAKTcpServerDeviceController *ui;

    QComboBox *serverHostComboBox;
    QLineEdit *serverPortLineEdit;
    QComboBox *clientHostComboBox;
};

#endif
