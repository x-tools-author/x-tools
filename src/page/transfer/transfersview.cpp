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
#include "socket/udp/udpclienttransferview.h"
#include "socket/udp/udpservertransferview.h"

#ifdef X_ENABLE_SERIAL_PORT
#include "serialport/serialporttransferview.h"
#endif
#ifdef X_ENABLE_WEB_SOCKET
#include "socket/websocket/wsclienttransferview.h"
#include "socket/websocket/wsservertransferview.h"
#endif

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

    auto udpClientTransfer = new UdpClientTransferView(this);
    ctx.name = tr("UDP Client");
    ctx.view = udpClientTransfer;
    m_transfersContextList.append(ctx);

    auto udpServerTransfer = new UdpServerTransferView(this);
    ctx.name = tr("UDP Server");
    ctx.view = udpServerTransfer;
    m_transfersContextList.append(ctx);

#ifdef X_ENABLE_WEB_SOCKET
    auto wsClientTransfer = new WsClientTransferView(this);
    ctx.name = tr("WebSocket Client");
    ctx.view = wsClientTransfer;
    m_transfersContextList.append(ctx);

    auto wsServerTransfer = new WsServerTransferView(this);
    ctx.name = tr("WebSocket Server");
    ctx.view = wsServerTransfer;
    m_transfersContextList.append(ctx);
#endif

    for (auto &ctx : m_transfersContextList) {
        addTab(ctx.view, ctx.name);
    }
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