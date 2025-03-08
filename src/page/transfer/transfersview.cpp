/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "transfersview.h"

#include "socket/tcp/tcpclienttransferview.h"
#include "socket/tcp/tcpservertransferview.h"

#ifdef X_ENABLE_SERIAL_PORT
#include "serialport/serialporttransferview.h"
#endif
// #ifdef X_ENABLE_WEB_SOCKET
// #include "device/websocketclient.h"
// #include "device/websocketclientui.h"
// #include "device/websocketserver.h"
// #include "device/websocketserverui.h"
// #include "page/transfer/websocketclienttransfer.h"
// #include "page/transfer/websocketclienttransferui.h"
// #include "page/transfer/websocketservertransfer.h"
// #include "page/transfer/websocketservertransferui.h"
// #endif

// #ifdef X_ENABLE_BLUETOOTH
// #include "device/blecentral.h"
// #include "device/blecentralui.h"
// #endif

TransfersView::TransfersView(QWidget *parent)
    : QTabWidget(parent)
{
    TransfersContext ctx;
#ifdef X_ENABLE_SERIAL_PORT
    auto serialPortTransfers = new SerialPortTransferView(this);
    ctx.name = tr("Serial Port");
    ctx.view = serialPortTransfers;
    m_transfersContextList.append(ctx);
#endif

    auto tcpClientTransfer = new TcpClientTransferView(this);
    ctx.name = tr("TCP Client");
    ctx.view = tcpClientTransfer;
    m_transfersContextList.append(ctx);

    auto tcpServerTransfer = new TcpServerTransferView(this);
    ctx.name = tr("TCP Server");
    ctx.view = tcpServerTransfer;
    m_transfersContextList.append(ctx);

    for (auto &ctx : m_transfersContextList) {
        addTab(ctx.view, ctx.name);
    }
#if 0
    addTab(m_udpClientTransferUi, tr("UDP Client"));
    addTab(m_udpServerTransferUi, tr("UDP Server"));
    addTab(m_tcpClientTransferUi, tr("TCP Client"));
    addTab(m_tcpServerTransferUi, tr("TCP Server"));
#ifdef X_ENABLE_WEB_SOCKET
    addTab(m_wsClientTransferUi, tr("WebSocket Client"));
    addTab(m_wsServerTransferUi, tr("WebSocket Server"));
#endif
#ifdef X_ENABLE_SERIAL_PORT
    m_serialPortTransferUi->setupIO(m_serialPortTransfer);
#endif
    m_udpClientTransferUi->setupIO(m_udpClientTransfer);
    m_udpServerTransferUi->setupIO(m_udpServerTransfer);
    m_tcpClientTransferUi->setupIO(m_tcpClientTransfer);
    m_tcpServerTransferUi->setupIO(m_tcpServerTransfer);
#ifdef X_ENABLE_WEB_SOCKET
    m_wsClientTransferUi->setupIO(m_wsClientTransfer);
    m_wsServerTransferUi->setupIO(m_wsServerTransfer);
#endif
#endif
}

TransfersView::~TransfersView() {}

void TransfersView::startAll()
{
    for (auto &ctx : m_transfersContextList) {
        ctx.view->startAll();
    }
}

void TransfersView::stopAll()
{
    for (auto &ctx : m_transfersContextList) {
        ctx.view->stopAll();
    }
}

QVariantMap TransfersView::save() const
{
    QVariantMap map;
    for (auto &ctx : m_transfersContextList) {
        map.insert(ctx.view->metaObject()->className(), ctx.view->save());
    }

    return map;
}

void TransfersView::load(const QVariantMap &data)
{
    if (data.isEmpty()) {
        return;
    }

    for (auto &ctx : m_transfersContextList) {
        ctx.view->load(data.value(ctx.view->metaObject()->className()).toMap());
    }
}

void TransfersView::inputBytes(const QByteArray &bytes)
{
    for (auto &ctx : m_transfersContextList) {
        ctx.view->inputBytes(bytes);
    }
}