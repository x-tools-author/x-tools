/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKWEBSOCKETSERVERDEVICECONTROLLER_HH
#define SAKWEBSOCKETSERVERDEVICECONTROLLER_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>
#include <QWebSocket>

namespace Ui {
    class SAKWebSocketServerDeviceController;
}
/// @brief web socket 服务器设备控制类
class SAKWebSocketServerDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKWebSocketServerDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketServerDeviceController();

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
