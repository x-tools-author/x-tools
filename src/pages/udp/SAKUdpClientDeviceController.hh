/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
