/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QJsonObject>
#include <QThread>

namespace xCoAP {

class CoAPResourceModel;
class CoAPServerPrivate;
class CoAPServer : public QThread
{
    Q_OBJECT
    CoAPServerPrivate *d;

public:
    explicit CoAPServer(QObject *parent = nullptr);
    ~CoAPServer();

    void startServer(const QJsonObject &parameters);
    void stopServer();
    void setCoAPResourceModel(CoAPResourceModel *model);

protected:
    void run() override;
};

} // namespace xCoAP
