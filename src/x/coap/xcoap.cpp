/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xcoap.h"
#include "ui_xcoap.h"

#include <QAction>
#include <QMenu>

#include "coapclientui.h"
#include "coapserverui.h"

namespace Ui {
class xCoAP;
}

namespace xCoAP {

class xCoAPPrivate : public QObject
{
public:
    xCoAPPrivate(xCoAP* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::xCoAP();
        ui->setupUi(q);

        m_toolButtonMenu = new QMenu(q);
        m_clientAction = m_toolButtonMenu->addAction("Client View");
        m_serverAction = m_toolButtonMenu->addAction("Server View");
        m_clientAction->setCheckable(true);
        m_serverAction->setCheckable(true);
        connect(m_clientAction, &QAction::triggered, q, [=]() { onClientActionTriggered(); });
        connect(m_serverAction, &QAction::triggered, q, [=]() { onServerActionTriggered(); });

        m_client = new CoAPClientUi(q);
        m_server = new CoAPServerUi(q);
        ui->tabWidgetClient->addTab(m_client, "Client");
        ui->tabWidgetServer->addTab(m_server, "Server");
    }
    ~xCoAPPrivate() {}

public:
    Ui::xCoAP* ui{nullptr};
    CoAPClientUi* m_client{nullptr};
    CoAPServerUi* m_server{nullptr};
    QMenu* m_toolButtonMenu{nullptr};
    QAction* m_clientAction{nullptr};
    QAction* m_serverAction{nullptr};

private:
    void onClientActionTriggered()
    {
        if (this->m_clientAction->isChecked()) {
            ui->tabWidgetClient->show();
        } else {
            // client and server cannot be hidden at the same time
            if (this->m_serverAction->isChecked()) {
                ui->tabWidgetClient->setVisible(this->m_clientAction->isChecked());
            } else {
                this->m_clientAction->setChecked(true);
            }
        }
    }
    void onServerActionTriggered()
    {
        if (this->m_serverAction->isChecked()) {
            ui->tabWidgetServer->show();
        } else {
            // client and server cannot be hidden at the same time
            if (this->m_clientAction->isChecked()) {
                ui->tabWidgetServer->setVisible(this->m_serverAction->isChecked());
            } else {
                this->m_serverAction->setChecked(true);
            }
        }
    }

private:
    xCoAP* q{nullptr};
};

xCoAP::xCoAP(QWidget* parent)
    : QWidget(parent)
{
    d = new xCoAPPrivate(this);
}

xCoAP::~xCoAP() {}

QJsonObject xCoAP::save()
{
    QJsonObject obj;
    return obj;
}

void xCoAP::load(const QJsonObject& obj) {}

QMenu* xCoAP::toolButtonMenu()
{
    return d->m_toolButtonMenu;
}

} // namespace xCoAP
