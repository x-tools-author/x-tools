/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "transfersview.h"

#include "common/transfermodel.h"

#include "socket/tcp/tcpclienttransferview.h"
#include "socket/tcp/tcpservertransferview.h"
#include "socket/udp/udpclienttransferview.h"
#include "socket/udp/udpservertransferview.h"

#if X_ENABLE_SERIALPORT
#include "serialport/serialporttransferview.h"
#endif
#ifdef X_ENABLE_WEBSOCKETS
#include "socket/websocket/websocketclienttransferview.h"
#include "socket/websocket/websocketservertransferview.h"
#endif

TransfersView::TransfersView(QWidget *parent)
    : QTabWidget(parent)
{
    TransfersContext ctx;
#if X_ENABLE_SERIALPORT
    auto serialPortTransfers = new SerialPortTransferView(this);
    ctx.name = tr("Serial Port");
    ctx.view = serialPortTransfers;
    m_transfersContextList.append(ctx);
    auto serialPortModel = qobject_cast<TransferModel *>(serialPortTransfers->tableModel());
    connect(serialPortModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(serialPortModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);
#endif

    auto tcpClientTransfer = new TcpClientTransferView(this);
    ctx.name = tr("TCP Client");
    ctx.view = tcpClientTransfer;
    m_transfersContextList.append(ctx);
    auto tcpClientModel = qobject_cast<TransferModel *>(tcpClientTransfer->tableModel());
    connect(tcpClientModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(tcpClientModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);

    auto tcpServerTransfer = new TcpServerTransferView(this);
    ctx.name = tr("TCP Server");
    ctx.view = tcpServerTransfer;
    m_transfersContextList.append(ctx);
    auto tcpServerModel = qobject_cast<TransferModel *>(tcpServerTransfer->tableModel());
    connect(tcpServerModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(tcpServerModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);

    auto udpClientTransfer = new UdpClientTransferView(this);
    ctx.name = tr("UDP Client");
    ctx.view = udpClientTransfer;
    m_transfersContextList.append(ctx);
    auto udpClientModel = qobject_cast<TransferModel *>(udpClientTransfer->tableModel());
    connect(udpClientModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(udpClientModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);

    auto udpServerTransfer = new UdpServerTransferView(this);
    ctx.name = tr("UDP Server");
    ctx.view = udpServerTransfer;
    m_transfersContextList.append(ctx);
    auto udpServerModel = qobject_cast<TransferModel *>(udpServerTransfer->tableModel());
    connect(udpServerModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(udpServerModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);

#ifdef X_ENABLE_WEBSOCKETS
    auto wsClientTransfer = new WebSocketClientTransferView(this);
    ctx.name = tr("WebSocket Client");
    ctx.view = wsClientTransfer;
    m_transfersContextList.append(ctx);
    auto wsClientModel = qobject_cast<TransferModel *>(wsClientTransfer->tableModel());
    connect(wsClientModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(wsClientModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);

    auto wsServerTransfer = new WebSocketServerTransferView(this);
    ctx.name = tr("WebSocket Server");
    ctx.view = wsServerTransfer;
    m_transfersContextList.append(ctx);
    auto wsServerModel = qobject_cast<TransferModel *>(wsServerTransfer->tableModel());
    connect(wsServerModel, &TransferModel::bytesRead, this, &TransfersView::bytesRead);
    connect(wsServerModel, &TransferModel::bytesWritten, this, &TransfersView::bytesWritten);
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
