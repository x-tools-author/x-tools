/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "SAKWebSocketClientTool.h"

#include "SAKInterface.h"

#define WS_ERR_SIGNAL void (QWebSocket::*)(QAbstractSocket::SocketError)

SAKWebSocketClientTool::SAKWebSocketClientTool(QObject *parent)
    : SAKSocketClientTool{"sak.websocketclienttool", parent} {}

bool SAKWebSocketClientTool::initialize(QString &errStr) {
  Q_UNUSED(errStr)
  mWebSocket = new QWebSocket();
  connect(mWebSocket, &QWebSocket::connected, mWebSocket, [=]() {
    QString address = mWebSocket->localAddress().toString();
    QString port = QString::number(mWebSocket->localPort());
    this->mBindingIpPort = address + ":" + port;
    emit bindingIpPortChanged();

    QString ip = mWebSocket->peerAddress().toString();
    this->mPeerInfo = ip + ":" + QString::number(mWebSocket->peerPort());
  });

  connect(mWebSocket, &QWebSocket::disconnected, mWebSocket, [=]() {
    QString errStr = mWebSocket->errorString();
    if (!errStr.isEmpty()) {
      errStr = "Disconected from server:" + errStr;
      outputMessage(QtInfoMsg, errStr);
    }

    exit();
  });

  connect(mWebSocket, &QWebSocket::binaryFrameReceived, mWebSocket,
          [=](const QByteArray &message) {
            QByteArray ba = SAKInterface::arrayToHex(message, ' ');
            QString hex = QString::fromLatin1(ba);
            QString info = mBindingIpPort + "<-" + this->mPeerInfo + ":" + hex;
            outputMessage(QtInfoMsg, info);
            emit bytesOutputted(message, rxJsonObject());
          });

  connect(mWebSocket, &QWebSocket::textMessageReceived, mWebSocket,
          [=](QString message) {
            QString info =
                mBindingIpPort + "<-" + this->mPeerInfo + ":" + message;
            outputMessage(QtInfoMsg, info);
            emit bytesOutputted(message.toUtf8(), rxJsonObject());
          });

#if QT_VERSION >= QT_VERSION_CHECK(6, 5, 0)
  connect(mWebSocket, static_cast<WS_ERR_SIGNAL>(&QWebSocket::errorOccurred),
#else
  connect(mWebSocket, static_cast<WS_ERR_SIGNAL>(&QWebSocket::error),
#endif
          mWebSocket, [=](QAbstractSocket::SocketError error) {
            Q_UNUSED(error);
            QString errStr = mWebSocket->errorString();
            outputMessage(QtInfoMsg, errStr);
            emit errorOccured(errStr);
          });

  QString address = "ws://" + mServerIp + ":" + QString::number(mServerPort);
  qCDebug(mLoggingCategory) << "Server url: " + address;
  mWebSocket->open(address);

  return true;
}

void SAKWebSocketClientTool::writeBytes(const QByteArray &bytes,
                                        const QVariant &context) {
  Q_UNUSED(context);
  qint64 ret = -1;
  QString hex;
  if (mMessageType == 0) {
    hex = QString::fromLatin1(SAKInterface::arrayToHex(bytes, ' '));
    ret = mWebSocket->sendBinaryMessage(bytes);
  } else {
    hex = QString::fromUtf8(bytes);
    ret = mWebSocket->sendTextMessage(QString::fromUtf8(bytes));
  }

  if (ret == -1) {
    outputMessage(QtWarningMsg, mWebSocket->errorString());
  } else {
    QString info = mBindingIpPort + "<-" + this->mPeerInfo + ":" + hex;
    outputMessage(QtInfoMsg, info);
    emit bytesInputted(bytes, txJsonObject());
  }
}

void SAKWebSocketClientTool::uninitialize() {
  mWebSocket->close();
  mWebSocket->deleteLater();
  mWebSocket = nullptr;
}
