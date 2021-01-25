/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKBLUETOOTHCLIENTDEVICECONTROLLER_HH
#define SAKBLUETOOTHCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKBluetoothClientDeviceController;
}

class SAKDebugPage;
class SAKBluetoothClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct BluetoothClientParameters {
        QString localHost;
        quint16 localPort;
        QString serverHost;
        quint16 serverPort;
        bool specifyClientAddressAndPort;
        bool allowAutomaticConnection;
    };

    SAKBluetoothClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKBluetoothClientDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;
    void setClientInfo(QString info);
private:
    QMutex mParametersMutex;
    BluetoothClientParameters mParameters;
private:
    Ui::SAKBluetoothClientDeviceController *mUi;
    QComboBox *mLocalhostComboBox;
    QLineEdit *mLocalPortlineEdit;
    QCheckBox *mSpecifyClientAddressAndPort;
    QCheckBox *mAutomaticConnectionCheckBox;
    QLineEdit *mClientInfoLineEdit;
    QLineEdit *mServerHostLineEdit;
    QLineEdit *mServerPortLineEdit;
private slots:
    void on_localhostComboBox_currentIndexChanged(int index);
    void on_localPortlineEdit_textChanged(const QString &arg1);
    void on_specifyClientAddressAndPort_clicked();
    void on_serverHostLineEdit_textChanged(const QString &arg1);
    void on_serverPortLineEdit_textChanged(const QString &arg1);
    void on_automaticConnectionCheckBox_clicked();
};
Q_DECLARE_METATYPE(SAKBluetoothClientDeviceController::BluetoothClientParameters);
#endif
