/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKTCPSERVERDEVICECONTROLLER_HH
#define SAKTCPSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>
#include <QStandardItemModel>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKTcpServerDeviceController;
}

class SAKDebugPage;
class SAKTcpServerDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct TcpServerParameters {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
    };

    SAKTcpServerDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;

    void addClient(QString host, quint16 port, QTcpSocket *socket);
    void removeClient(QTcpSocket *socket);
private:
    QMutex mParametersMutex;
    TcpServerParameters mParameters;
    QStandardItemModel mClientStandardItemModel;
private:
    Ui::SAKTcpServerDeviceController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
private slots:
    void on_serverhostComboBox_currentTextChanged(const QString &arg1);
    void on_serverPortLineEdit_textChanged(const QString &arg1);
    void on_clientHostComboBox_currentTextChanged(const QString &arg1);
};
Q_DECLARE_METATYPE(SAKTcpServerDeviceController::TcpServerParameters);
#endif
