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

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKUdpClientDeviceController;
}

class SAKDebugPage;
class SAKUdpClientDevice;
class SAKUdpClientAdvanceSettingWidget;
class SAKUdpClientDeviceController : public SAKDebugPageController
{
    Q_OBJECT
public:
    struct UdpClientParameters{
        QString localHost;
        quint16 localPort;
        QString targetHost;
        quint16 targetPort;
        bool specifyClientAddressAndPort;
    };

    SAKUdpClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKUdpClientDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;

    void setUdpDevice(SAKUdpClientDevice *device);
    void setClientInfo(QString info);
private:
    QMutex mParametersMutex;
    UdpClientParameters mParameters;
    SAKUdpClientAdvanceSettingWidget *mUdpAdvanceSettingWidget;
private:
    Ui::SAKUdpClientDeviceController *mUi;
    QComboBox *mLocalhostComboBox;
    QLineEdit *mLocalPortlineEdit;
    QCheckBox *mSpecifyClientAddressAndPort;
    QLineEdit *mBoundInfoLineEdit;
    QLineEdit *mTargetHostLineEdit;
    QLineEdit *mTargetPortLineEdit;
    QPushButton *mAdvanceUdpPushButton;
private slots:
    void on_advanceUdpPushButton_clicked();
    void on_localhostComboBox_currentTextChanged(const QString &arg1);
    void on_localPortlineEdit_textChanged(const QString &arg1);
    void on_targetHostLineEdit_textChanged(const QString &arg1);
    void on_targetPortLineEdit_textChanged(const QString &arg1);
};
Q_DECLARE_METATYPE(SAKUdpClientDeviceController::UdpClientParameters);
#endif
