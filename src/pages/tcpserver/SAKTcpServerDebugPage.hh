/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKTCPSERVERDEBUGPAGE_HH
#define SAKTCPSERVERDEBUGPAGE_HH

#include "SAKDebugPage.hh"

class SAKTcpServerDevice;
class SAKTcpServerDeviceController;
class SAKTcpServerDebugPage : public SAKDebugPage
{
    Q_OBJECT
public:
    SAKTcpServerDebugPage(QWidget *parent = Q_NULLPTR);
    ~SAKTcpServerDebugPage();

private:
    SAKTcpServerDevice *tcpServerDevice;
    SAKTcpServerDeviceController *tcpServerDeviceController;

    void setUiEnable(bool enable);
    void changeDeviceStatus(bool opened);
    void tryWrite(QByteArray data);

    void afterBytesRead(QByteArray data, QString host, quint16 port);
    void afterBytesWritten(QByteArray data, QString host, quint16 port);

private:
    void openOrColoseDevice() final;
    void refreshDevice() final;
    QWidget *controllerWidget() final;

signals:
    void writeBytesRequest(QByteArray data, QString host, quint16 port);
};

#endif
