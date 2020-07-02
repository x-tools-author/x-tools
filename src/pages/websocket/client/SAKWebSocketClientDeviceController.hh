/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKWebSocketClientDeviceController_HH
#define SAKWebSocketClientDeviceController_HH

#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QComboBox>

namespace Ui {
    class SAKWebSocketClientDeviceController;
}
/// @brief web socket 客户端控制类
class SAKWebSocketClientDeviceController:public QWidget
{
    Q_OBJECT
public:
    SAKWebSocketClientDeviceController(QWidget *parent = Q_NULLPTR);
    ~SAKWebSocketClientDeviceController();

    QString serverAddress();
    void setUiEnable(bool enable);
    quint32 sendingType();
private:
    QMutex uiMutex;
private:
    Ui::SAKWebSocketClientDeviceController *ui;
    QLineEdit *serverAddressLineEdit;
    QComboBox *sendingTypeComboBox;
};

#endif
