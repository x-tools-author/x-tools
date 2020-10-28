/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QMetaEnum>
#include <QCoreApplication>

#include "SAKTestDebugPage.hh"
#include "SAKUdpClientDebugPage.hh"
#include "SAKUdpServerDebugPage.hh"
#include "SAKTcpClientDebugPage.hh"
#include "SAKTcpServerDebugPage.hh"
#include "SAKSerialPortDebugPage.hh"
#include "SAKWebSocketClientDebugPage.hh"
#include "SAKWebSocketServerDebugPage.hh"
#include "SAKMainWindowDebugPageFactory.hh"

SAKMainWindowDebugPageFactory *SAKMainWindowDebugPageFactory::instancePtr = Q_NULLPTR;
QMutex* SAKMainWindowDebugPageFactory::singletonMutex = new QMutex;
SAKMainWindowDebugPageFactory::SAKMainWindowDebugPageFactory(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;

    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeTest, SAKTestDebugPage::staticMetaObject, tr("Test")});
#ifdef SAK_IMPORT_COM_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeCOM, SAKSerialPortDebugPage::staticMetaObject, tr("COM")});
#endif
#ifdef SAK_IMPORT_HID_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeHID, SAKHIDDebugPage::staticMetaObject, tr("HID")});
#endif
#ifdef SAK_IMPORT_USB_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeUSB, SAKUSBDebugPage::staticMetaObject, tr("USB")});
#endif
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeUdpClient, SAKUdpClientDebugPage::staticMetaObject, tr("UDP-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeUdpServer, SAKUdpServerDebugPage::staticMetaObject, tr("UDP-S")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeTCPClient, SAKTcpClientDebugPage::staticMetaObject, tr("TCP-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeTCPServer, SAKTcpServerDebugPage::staticMetaObject, tr("TCP-S")});
#ifdef SAK_IMPORT_MODULE_SSLSOCKET
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeSslSocketClient, SAKSslSocketClientDebugPage::staticMetaObject, tr("SSL-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeSslSocketServer, SAKSslSocketServerDebugPage::staticMetaObject, tr("SSL-S")});
#endif
#ifdef SAK_IMPORT_SCTP_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeSCTPClient, SAKSCTPClientDebugPage::staticMetaObject, tr("SCTP-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeSCTPServer, SAKSCTPServerDebugPage::staticMetaObject, tr("SCTP-S")});
#endif
#ifdef SAK_IMPORT_BLUETOOTH_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeBluetoothClient, SAKBluetoothClientDebugPage::staticMetaObject, tr("Bluetooth-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeBluetoothServer, SAKBluetoothServerDebugPage::staticMetaObject, tr("Bluetooth-S")});
#endif
#ifdef SAK_IMPORT_WEBSOCKET_MODULE
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeWebSocketClient, SAKWebSocketClientDebugPage::staticMetaObject, tr("WS-C")});
    mDebugPageMetaInfoList.append(SAKDebugPageMetaInfo{DebugPageTypeWebSocketServer, SAKWebSocketServerDebugPage::staticMetaObject, tr("WS-S")});
#endif
}

SAKMainWindowDebugPageFactory::~SAKMainWindowDebugPageFactory()
{
    instancePtr = Q_NULLPTR;
}

SAKMainWindowDebugPageFactory *SAKMainWindowDebugPageFactory::instance()
{
    singletonMutex->lock();
    if (!instancePtr){
        new SAKMainWindowDebugPageFactory(qApp);
    }
    singletonMutex->unlock();

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Can not alloc memory for SAKMainWindowDebugPageFactory instance!");

    return instancePtr;
}

QWidget *SAKMainWindowDebugPageFactory::debugPageFromDebugPageType(int type)
{
    QWidget *widget = Q_NULLPTR;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (auto var : mDebugPageMetaInfoList){
        if (var.debugPageType == type){
            for (int i = 0; i < metaEnum.keyCount(); i++){
                if (var.debugPageType == metaEnum.value(i)){
                    widget = qobject_cast<QWidget*>(var.metaObject.newInstance());
                    break;
                }
            }
        }
    }

    Q_ASSERT_X(widget, __FUNCTION__, "Unknow debug page type!");
    return widget;
}

QString SAKMainWindowDebugPageFactory::debugPageTitleFromDebugPageType(int type)
{
    QString title;
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    for (auto var : mDebugPageMetaInfoList){
        if (var.debugPageType == type){
            for (int i = 0; i < metaEnum.keyCount(); i++){
                if (var.debugPageType == metaEnum.value(i)){
                    title = var.defaultTitle;
                    break;
                }
            }
        }
    }

    if (title.isEmpty()){
        title = QString("UnknowDebugPage");
        Q_ASSERT_X(false, __FUNCTION__, "Unknow debug page type!");
    }

    return title;
}

QString SAKMainWindowDebugPageFactory::debugPageTypeNameFromDebugPageType(int type)
{
    QMetaEnum metaEnum = QMetaEnum::fromType<SAKEnumDebugPageType>();
    QString name = metaEnum.valueToKey(type);

    Q_ASSERT_X(name.isEmpty(), __FUNCTION__, "Unknow debug page type!");

    return name;
}
