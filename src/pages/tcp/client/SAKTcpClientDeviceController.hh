/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPCLIENTDEVICECONTROLLER_HH
#define SAKTCPCLIENTDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKTcpClientDeviceController;
}

class SAKDebugPage;
class SAKTcpClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct TcpClientParameters {
        QString localHost;
        quint16 localPort;
        QString serverHost;
        quint16 serverPort;
        bool specifyClientAddressAndPort;
        bool allowAutomaticConnection;
    };

    SAKTcpClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTcpClientDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;
    void setClientInfo(QString info);
private:
    QMutex mParametersMutex;
    TcpClientParameters mParameters;
private:
    Ui::SAKTcpClientDeviceController *mUi;
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
Q_DECLARE_METATYPE(SAKTcpClientDeviceController::TcpClientParameters);
#endif
