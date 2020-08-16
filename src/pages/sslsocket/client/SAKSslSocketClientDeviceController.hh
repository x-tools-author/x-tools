/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSSLSOCKETCLIENTDEVICECONTROLLER_HH
#define SAKSSLSOCKETCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKSslSocketClientDeviceController;
}

class SAKDebugPage;
class SAKSslSocketClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct SslSocketClientParameters {
        QString localHost;
        quint16 localPort;
        QString serverHost;
        quint16 serverPort;
        bool specifyClientAddressAndPort;
    };

    SAKSslSocketClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKSslSocketClientDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;

    void setClientInfo(QString info);
private:
    QMutex mParametersMutex;
    SslSocketClientParameters mParameters;
private:
    Ui::SAKSslSocketClientDeviceController *mUi;
    QComboBox *mLocalhostComboBox;
    QLineEdit *mLocalPortlineEdit;
    QCheckBox *mSpecifyClientAddressAndPort;
    QLineEdit *mClientInfoLineEdit;
    QLineEdit *mServerHostLineEdit;
    QLineEdit *mServerPortLineEdit;
private slots:
    void on_localhostComboBox_currentIndexChanged(int index);
    void on_localPortlineEdit_textChanged(const QString &arg1);
    void on_specifyClientAddressAndPort_clicked();
    void on_serverHostLineEdit_textChanged(const QString &arg1);
    void on_serverPortLineEdit_textChanged(const QString &arg1);
};
Q_DECLARE_METATYPE(SAKSslSocketClientDeviceController::SslSocketClientParameters);
#endif
