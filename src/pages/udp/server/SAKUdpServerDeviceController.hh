/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUDPSERVERDEVICECONTROLLER_HH
#define SAKUDPSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QVector>
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKUdpServerDeviceController;
}

class SAKDebugPage;
/// @brief Udp server control panel
class SAKUdpServerDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct UdpServerParameters {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;

        QStringList clients;
    };

    SAKUdpServerDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKUdpServerDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;

    void addClient(QString host, quint16 port);
private:
    QMutex mParametersMutex;
    UdpServerParameters mParameters;
private:
    Ui::SAKUdpServerDeviceController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
private slots:
    void on_clientHostComboBox_currentTextChanged(const QString &arg1);
    void on_clearPushButton_clicked();
    void on_serverhostComboBox_currentTextChanged(const QString &arg1);
    void on_serverPortLineEdit_textChanged(const QString &arg1);
};
Q_DECLARE_METATYPE(SAKUdpServerDeviceController::UdpServerParameters);
#endif
