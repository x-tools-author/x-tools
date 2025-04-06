/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QLocalSocket>

#include "deviceui.h"

namespace Ui {
class LocalServerUi;
}

class LocalSocket;
class LocalServer;
class LocalServerUi : public DeviceUi
{
    Q_OBJECT
public:
    explicit LocalServerUi(QWidget *parent = nullptr);
    ~LocalServerUi() override;

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setUiEnabled(bool enabled) override;

protected:
    Device *newDevice() override;

private:
    void onSocketConnected(QLocalSocket *socket, const QString &socketName);
    void onSocketDisconnected(QLocalSocket *socket);
    void onTargetChanged();

private:
    Ui::LocalServerUi *ui{nullptr};
    LocalServer *m_server{nullptr};
};