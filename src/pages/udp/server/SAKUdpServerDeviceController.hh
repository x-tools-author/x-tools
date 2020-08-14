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
#include <QCheckBox>
#include <QComboBox>
#include <QTcpSocket>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKUdpServerDeviceController;
}

class SAKDebugPage;
// Udp server control panel
class SAKUdpServerDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    SAKUdpServerDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKUdpServerDeviceController();

    QString serverHost();
    quint16 serverPort();

    QString currentClientHost();
    quint16 currentClientPort();

    void refresh();
    void setUiEnable(bool enable);

    void addClientSafely(QString host, quint16 port);
    bool hasNoClient();
private:
    QString mCurrentHost;
    quint16 mCurrentPort;
    QString mServerHost;
    quint16 mServerPort;
    QMutex mParametersMutex;
    bool mHasNoClient;
private:
    void addClientSafelyActually(QString host, quint16 port);
signals:
    void requestAddClient(QString host, quint16 port);
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

#endif
