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
#include <QToolButton>

#include "utilities/iconengine.h"

#include "coapclientui.h"
#include "coapglobal.h"
#include "coapserverui.h"

namespace Ui {
class xCoAP;
}

namespace xCoAP {

struct xCoAPSettingKeys
{
    const QString clientViewVisible{"clientViewVisible"};
    const QString serverViewVisible{"serverViewVisible"};
    const QString client{"client"};
    const QString server{"server"};
    const QString globalSettings{"globalSettings"};
};

class xCoAPPrivate : public QObject
{
public:
    xCoAPPrivate(xCoAP* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::xCoAP();
        ui->setupUi(q);

        // Tool button menu
        m_toolButtonMenu = new QMenu(q);
        QAction* exportAction = m_toolButtonMenu->addAction("Export CoAP Configuration");
        exportAction->setIcon(xIcon(":res/icons/save.svg"));
        QAction* importAction = m_toolButtonMenu->addAction("Import CoAP Configuration");
        importAction->setIcon(xIcon(":res/icons/file_open.svg"));
        m_toolButtonMenu->addSeparator();
        QAction* settingAction = m_toolButtonMenu->addAction(tr("Global Settings"));
        settingAction->setIcon(xIcon(":res/icons/settings.svg"));
        connect(exportAction, &QAction::triggered, q, [=]() { onExportActionTriggered(); });
        connect(importAction, &QAction::triggered, q, [=]() { onImportActionTriggered(); });
        connect(settingAction, &QAction::triggered, q, [=]() { onSettingsActionTriggered(); });

        // Client and Server control tool buttons
        m_clientCtrlToolButton = new QToolButton(q);
        m_serverCtrlToolButton = new QToolButton(q);
        m_clientCtrlToolButton->setToolTip("Show/Hide Client View");
        m_serverCtrlToolButton->setToolTip("Show/Hide Server View");
        m_clientCtrlToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        m_serverCtrlToolButton->setToolButtonStyle(Qt::ToolButtonIconOnly);
        ui->tabWidgetClient->setCornerWidget(m_serverCtrlToolButton, Qt::TopRightCorner);
        ui->tabWidgetServer->setCornerWidget(m_clientCtrlToolButton, Qt::TopRightCorner);
        connect(m_clientCtrlToolButton, &QToolButton::clicked, q, [=]() {
            onClientCtrlToolButtonClicked();
        });
        connect(m_serverCtrlToolButton, &QToolButton::clicked, q, [=]() {
            onServerCtrlToolButtonClicked();
        });

        // Client and Server tabs
        m_client = new CoAPClientUi(q);
        m_server = new CoAPServerUi(q);
        ui->tabWidgetClient->addTab(m_client, tr("Client"));
        ui->tabWidgetClient->addTab(m_client->optionView(), tr("Options"));
        ui->tabWidgetClient->addTab(m_client->payloadView(), tr("Payloads"));
        ui->tabWidgetServer->addTab(m_server, tr("Server"));
        ui->tabWidgetServer->addTab(m_server->resourceView(), tr("Resources"));
    }
    ~xCoAPPrivate() {}

public:
    Ui::xCoAP* ui{nullptr};
    CoAPClientUi* m_client{nullptr};
    CoAPServerUi* m_server{nullptr};
    QMenu* m_toolButtonMenu{nullptr};
    QToolButton* m_clientCtrlToolButton{nullptr};
    QToolButton* m_serverCtrlToolButton{nullptr};

private:
    void onClientCtrlToolButtonClicked()
    {
        bool isVisible = !ui->tabWidgetClient->isVisible();
        if (isVisible) {
            ui->tabWidgetClient->show();
        } else {
            // Client and Server can not be hidden at the same time
            if (ui->tabWidgetServer->isVisible()) {
                ui->tabWidgetClient->setVisible(isVisible);
            } else {
                ui->tabWidgetClient->show();
            }
        }

        QString clientCtrlBtnIcon = m_client->isVisible() ? ":res/icons/chevron_left.svg"
                                                          : ":res/icons/chevron_right.svg";
        m_clientCtrlToolButton->setIcon(xIcon(clientCtrlBtnIcon));
    }
    void onServerCtrlToolButtonClicked()
    {
        bool isVisible = !ui->tabWidgetServer->isVisible();
        if (isVisible) {
            ui->tabWidgetServer->show();
        } else {
            // Client and Server can not be hidden at the same time
            if (ui->tabWidgetClient->isVisible()) {
                ui->tabWidgetServer->setVisible(isVisible);
            } else {
                ui->tabWidgetServer->show();
            }
        }
        QString serverCtrlBtnIcon = m_server->isVisible() ? ":res/icons/chevron_right.svg"
                                                          : ":res/icons/chevron_left.svg";
        m_serverCtrlToolButton->setIcon(xIcon(serverCtrlBtnIcon));
    }
    void onExportActionTriggered() {}
    void onImportActionTriggered() {}
    void onSettingsActionTriggered() { gCoAPGlobal.showThenMoveToCenter(); }

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
    xCoAPSettingKeys keys;
    QJsonObject obj;
    obj.insert(keys.clientViewVisible, d->ui->tabWidgetClient->isVisible());
    obj.insert(keys.serverViewVisible, d->ui->tabWidgetServer->isVisible());
    obj.insert(keys.client, d->m_client->save());
    obj.insert(keys.server, d->m_server->save());
    obj.insert(keys.globalSettings, gCoAPGlobal.save());
    return obj;
}

void xCoAP::load(const QJsonObject& obj)
{
    xCoAPSettingKeys keys;
    bool clientVisible = obj.value(keys.clientViewVisible).toBool(true);
    bool serverVisible = obj.value(keys.serverViewVisible).toBool(true);
    if (!clientVisible && !serverVisible) {
        clientVisible = true;
        serverVisible = true;
    }

    d->ui->tabWidgetClient->setVisible(clientVisible);
    d->ui->tabWidgetServer->setVisible(serverVisible);
    QString clientCtrlBtnIcon = clientVisible ? ":res/icons/chevron_left.svg"
                                              : ":res/icons/chevron_right.svg";
    d->m_clientCtrlToolButton->setIcon(xIcon(clientCtrlBtnIcon));
    QString serverCtrlBtnIcon = serverVisible ? ":res/icons/chevron_right.svg"
                                              : ":res/icons/chevron_left.svg";
    d->m_serverCtrlToolButton->setIcon(xIcon(serverCtrlBtnIcon));
    d->m_client->load(obj.value(keys.client).toObject());
    d->m_server->load(obj.value(keys.server).toObject());
    gCoAPGlobal.load(obj.value(keys.globalSettings).toObject());
}

QMenu* xCoAP::toolButtonMenu()
{
    return d->m_toolButtonMenu;
}

} // namespace xCoAP
