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
/// @brief web socket 服务器设备控制类
class SAKWebSocketServerDeviceController:public SAKDebugPageController
{
    Q_OBJECT
public:
    SAKWebSocketServerDeviceController(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketServerDeviceController();

    QVariant parameters() final;
    QString serverHost();
    quint16 serverPort();
    QString currentClientHost();
    quint16 currentClientPort();
    quint32 sendingType();

    void refresh();
    void setUiEnable(bool enable);

    void addClient(QString host, quint16 port, QWebSocket *socket);
    void removeClient(QWebSocket *socket);
    void clearClient();
private:
    Ui::SAKWebSocketServerDeviceController *mUi;
    QComboBox *mServerHostComboBox;
    QLineEdit *mServerPortLineEdit;
    QComboBox *mClientHostComboBox;
    QComboBox *mSendingTypeComboBox;
    QMutex mUiMutex;
};

#endif
