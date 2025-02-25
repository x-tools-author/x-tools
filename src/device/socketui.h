/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "communicationui.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SocketUi;
}
QT_END_NAMESPACE

namespace xTools {

class Socket;
class SocketUi : public CommunicationUi
{
    Q_OBJECT
public:
    SocketUi(xIO::CommunicationType type, QWidget *parent = nullptr);
    ~SocketUi();

    QVariantMap save() const override;
    void load(const QVariantMap &parameters) override;
    void setupIO(AbstractIO *io) override;

signals:
    void currentClientChanged(const QString &flag);
    void invokeDisconnectAll();

protected:
    Socket *m_socket{nullptr};

protected:
    void setClientWidgetsVisible(bool visible);
    void setServerWidgetsVisible(bool visible);
    void setChannelWidgetsVisible(bool visible);
    void setAuthenticationWidgetsVisible(bool visible);
    void setWriteToWidgetsVisible(bool visible);
    void setMulticastWidgetsVisible(bool visible);

    void setClientWidgetsEnabled(bool enabled);
    void setServerWidgetsEnabled(bool enabled);
    void setChannelWidgetsEnabled(bool enabled);
    void setAuthenticationWidgetsEnabled(bool enabled);
    void setWriteToWidgetsEnabled(bool enabled);
    void setMulticastWidgetsEnabled(bool enabled);

    void setupClients(const QStringList &clients);

private:
    Ui::SocketUi *ui;
};

} // namespace xTools
