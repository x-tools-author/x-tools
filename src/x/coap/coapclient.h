/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <memory>

#include <QThread>

#include "coapcommon.h"

namespace xCoAP {

struct CoAPMsgItem;
class CoAPClientPrivate;
class CoAPClient : public QThread
{
    Q_OBJECT
    CoAPClientPrivate* d;

public:
    explicit CoAPClient(QObject* parent = nullptr);
    ~CoAPClient() override;

    void startClient(const QJsonObject& parameters);
    void stopClient();

signals:
    void errorOccurred(const QString& error);
    void invokeSendMessage(const QByteArray& payload,
                           const QByteArray& resource,
                           const QByteArray& option,
                           int code);
    void messageReceived(std::shared_ptr<CoAPMsgItem> request,
                         std::shared_ptr<CoAPMsgItem> response);

protected:
    void run() override;
};

} // namespace xCoAP
