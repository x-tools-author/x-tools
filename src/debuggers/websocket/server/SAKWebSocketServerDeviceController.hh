/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKWEBSOCKETSERVERDEVICECONTROLLER_HH
#define SAKWEBSOCKETSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QWebSocket>

#include "SAKDebugPageController.hh"

namespace Ui {
    class SAKWebSocketServerDeviceController;
}

class SAKDebugPage;
/// @brief Web socket server panel
class SAKWebSocketServerDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    struct WebSocketServerParameters {
        QString serverHost;
        quint16 serverPort;
        QString currentClientHost;
        quint16 currentClientPort;
        quint32 sendingType;
    };

    SAKWebSocketServerDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketServerDeviceController();

    QVariant parameters() final;
    void setUiEnable(bool opened) final;
    void refreshDevice() final;

    void addClient(QString host, quint16 port, QWebSocket *socket);
    void removeClient(QWebSocket *socket);
    void clearClient();
private:
    QMutex mParametersMutex;
    WebSocketServerParameters mParameters;
private:
    Ui::SAKWebSocketServerDeviceController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
    QComboBox *mSendingTypeComboBox;
private slots:
    void on_serverhostComboBox_currentTextChanged(const QString &arg1);
    void on_serverPortLineEdit_textChanged(const QString &arg1);
    void on_clientHostComboBox_currentTextChanged(const QString &arg1);
    void on_sendingTypeComboBox_currentIndexChanged(int index);
};
Q_DECLARE_METATYPE(SAKWebSocketServerDeviceController::WebSocketServerParameters);
#endif
