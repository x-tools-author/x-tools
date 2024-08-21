/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "WebSocketClient.h"

#include "../../xIO.h"

namespace xTools {

WebSocketClient::WebSocketClient(QObject *parent)
    : SocketClient(parent)
{}

WebSocketClient::~WebSocketClient() {}

QObject *WebSocketClient::initDevice()
{
    m_webSocket = new QWebSocket();
    connect(m_webSocket,
            &QWebSocket::textMessageReceived,
            m_webSocket,
            [this](const QString &message) { onTextMessageReceived(message); });
    connect(m_webSocket,
            &QWebSocket::binaryMessageReceived,
            m_webSocket,
            [this](const QByteArray &message) { onBinaryMessageReceived(message); });
    connect(m_webSocket, &QWebSocket::disconnected, m_webSocket, [this]() {
        emit errorOccurred("");
    });
    //TODO:qt6
#if QT_VERSION>= QT_VERSION_CHECK(6,5,0)
    connect(m_webSocket, &QWebSocket::errorOccurred, m_webSocket, [this]() {
        emit errorOccurred(m_webSocket->errorString());
    });
#else
    connect(m_webSocket,QOverload<QAbstractSocket::SocketError>::of(&QWebSocket::error),[this](QAbstractSocket::SocketError error){
        emit errorOccurred(qt_error_string(error));
    });
#endif

    QString url = QString("ws://%1:%2").arg(m_serverAddress).arg(m_serverPort);
    if (m_authentication) {
        QNetworkRequest request(url);
        QString username = m_username;
        QString password = m_password;
        QString concatenated = username + ":" + password;
        QByteArray data = concatenated.toLocal8Bit().toBase64();
        QString headerData = "Basic " + data;
        request.setRawHeader("Authorization", headerData.toLocal8Bit());

        qInfo() << "Request: " << request.url();
        qInfo() << "User: " << username << " Password: " << password;

        m_webSocket->open(request);
    } else {
        m_webSocket->open(url);
    }
    return m_webSocket;
}

void WebSocketClient::deinitDevice()
{
    m_webSocket->close();
    m_webSocket->deleteLater();
    m_webSocket = nullptr;
}

void WebSocketClient::writeBytes(const QByteArray &bytes)
{
    if (m_channel == static_cast<int>(xIO::WebSocketDataChannel::Text))
        m_webSocket->sendTextMessage(QString::fromUtf8(bytes));
    else if (m_channel == static_cast<int>(xIO::WebSocketDataChannel::Binary))
        m_webSocket->sendBinaryMessage(bytes);
    m_webSocket->sendTextMessage(QString::fromUtf8(bytes));
}

void WebSocketClient::onTextMessageReceived(const QString &message)
{
    QString flag = makeFlag(m_serverAddress, m_serverPort);
    emit bytesRead(message.toUtf8(), flag + "[T]");
}

void WebSocketClient::onBinaryMessageReceived(const QByteArray &message)
{
    QString flag = makeFlag(m_serverAddress, m_serverPort);
    emit bytesRead(message, flag + "[B]");
}

} // namespace xTools
