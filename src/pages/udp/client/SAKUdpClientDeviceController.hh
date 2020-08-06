/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPCLIENTDEVICECONTROLLER_HH
#define SAKUDPCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QPushButton>

namespace Ui {
    class SAKUdpClientDeviceController;
}

class SAKUdpClientDevice;
class SAKUdpClientAdvanceSettingWidget;
class SAKUdpClientDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKUdpClientDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientDeviceController();

    QString localHost();
    quint16 localPort();
    QString targetHost();
    quint16 targetPort();
    bool enableCustomLocalSetting();

    void refresh();
    void setUiEnable(bool enable);
    void setUdpDevice(SAKUdpClientDevice* device);
private:
    QMutex uiMutex;
private:
    Ui::SAKUdpClientDeviceController *ui;
    QComboBox *localhostComboBox;
    QLineEdit *localPortlineEdit;
    QCheckBox *enableLocalSettingCheckBox;
    QLineEdit *targetHostLineEdit;
    QLineEdit *targetPortLineEdit;
    QPushButton* advanceUdpPushButton;
private slots:
    void on_advanceUdpPushButton_clicked();
private:
    SAKUdpClientAdvanceSettingWidget* udpAdvanceSettingWidget;
};

#endif
