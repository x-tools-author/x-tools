/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKSSLSOCKETSERVERDEVICECONTROLLER_HH
#define SAKSSLSOCKETSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>
#include <QPushButton>
#include <QStandardItemModel>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKSslSocketServerDeviceController;
}

class SAKDebugPage;
class SAKDebugPageCommonSslConfigurationWidget;
class SAKSslSocketServerDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct SslSocketServerParameters {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
    };

    SAKSslSocketServerDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKSslSocketServerDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;

    void addClient(QString host, quint16 port, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
private:
    QMutex mParametersMutex;
    SslSocketServerParameters mParameters;
    QStandardItemModel mClientStandardItemModel;
    SAKDebugPageCommonSslConfigurationWidget *mSslConfiguration;
private:
    Ui::SAKSslSocketServerDeviceController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
    QPushButton *mSslConfigurationPushButton;
private slots:
    void on_serverhostComboBox_currentTextChanged(const QString &arg1);
    void on_serverPortLineEdit_textChanged(const QString &arg1);
    void on_clientHostComboBox_currentTextChanged(const QString &arg1);
    void on_sslConfigurationPushButton_clicked();
};
Q_DECLARE_METATYPE(SAKSslSocketServerDeviceController::SslSocketServerParameters);
#endif
