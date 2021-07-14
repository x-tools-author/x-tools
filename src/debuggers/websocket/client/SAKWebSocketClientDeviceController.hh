/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWebSocketClientDeviceController_HH
#define SAKWebSocketClientDeviceController_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKWebSocketClientDeviceController;
}

class SAKDebugPage;
/// @brief web socket client control panel
class SAKWebSocketClientDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct WebSocketClientParameters {
        QString serverAddress;
        quint32 sendingType;
    };

    SAKWebSocketClientDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketClientDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opend) final;

    void setClientInfo(QString info);
private:
    QMutex mParametersMutex;
    WebSocketClientParameters mParameters;
private:
    Ui::SAKWebSocketClientDeviceController *mUi;
    QLineEdit *mServerAddressLineEdit;
    QComboBox *mSendingTypeComboBox;
    QLineEdit *mClientInfoLineEdit;
private slots:
    void on_serverHostLineEdit_textChanged(const QString &arg1);
    void on_sendingTypeComboBox_currentIndexChanged(int index);
};
Q_DECLARE_METATYPE(SAKWebSocketClientDeviceController::WebSocketClientParameters);
#endif
