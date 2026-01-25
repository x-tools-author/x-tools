/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QThread>

namespace xCoAP {

class CoAPClientPrivate;
class CoAPClient : public QThread
{
    Q_OBJECT
    CoAPClientPrivate* d;

public:
    explicit CoAPClient(QObject* parent = nullptr);
    ~CoAPClient();

    void startClient(const QJsonObject& parameters);
    void stopClient();

    void getMessage(const QString& path);
    void postMessage(const QByteArray& payload, const QString& path);
    void putMessage(const QByteArray& payload, const QString& path);
    void deleteMessage(const QString& path);
    void fetchMessage(const QString& path);
    void patchMessage(const QByteArray& payload, const QString& path);
    void iPatchMessage(const QByteArray& payload, const QString& path);

signals:
    void errorOccurred(const QString& error);

protected:
    void run() override;
};

} // namespace xCoAP
