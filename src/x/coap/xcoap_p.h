/***************************************************************************************************
 * Copyright 2025-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "ui_xcoap.h"
#include "xcoap.h"

#include <QAction>
#include <QDesktopServices>
#include <QFileDialog>
#include <QMenu>
#include <QMessageBox>
#include <QToolButton>

#include "utilities/iconengine.h"

#include "coapclientui.h"
#include "coapglobal.h"
#include "coapserverui.h"

namespace Ui {
class xCoAP;
}

namespace xCoAP {

class xCoAPPrivate : public QObject
{
    Q_OBJECT
public:
    xCoAPPrivate(xCoAP* q_ptr)
        : QObject(q_ptr)
        , q(q_ptr)
    {
        ui = new Ui::xCoAP();
        ui->setupUi(q);

        // Tool button menu
        m_toolButtonMenu = new QMenu(q);
        QAction* exportAction = m_toolButtonMenu->addAction(tr("Export CoAP Configuration"));
        exportAction->setIcon(xIcon(":res/icons/save.svg"));
        QAction* importAction = m_toolButtonMenu->addAction(tr("Import CoAP Configuration"));
        importAction->setIcon(xIcon(":res/icons/file_open.svg"));
        m_toolButtonMenu->addSeparator();
        QAction* settingAction = m_toolButtonMenu->addAction(tr("Global Settings"));
        settingAction->setIcon(xIcon(":res/icons/settings.svg"));
        connect(exportAction, &QAction::triggered, q, [=]() { onExportActionTriggered(); });
        connect(importAction, &QAction::triggered, q, [=]() { onImportActionTriggered(); });
        connect(settingAction, &QAction::triggered, q, [=]() { onSettingsActionTriggered(); });
        QAction* cacheAction = m_toolButtonMenu->addAction(tr("Open Server Cache Directory"));
        cacheAction->setIcon(xIcon(":res/icons/folder.svg"));
        cacheAction->setVisible(false);
        connect(cacheAction, &QAction::triggered, q, [=]() { onOpenCachePath(); });

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
    }
    ~xCoAPPrivate() {}

public:
    Ui::xCoAP* ui{nullptr};
    CoAPClientUi* m_client{nullptr};
    CoAPServerUi* m_server{nullptr};
    QMenu* m_toolButtonMenu{nullptr};
    QToolButton* m_clientCtrlToolButton{nullptr};
    QToolButton* m_serverCtrlToolButton{nullptr};
    QSettings* m_settings{nullptr};

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
    void onExportActionTriggered() { q->exportSettings(QString()); }
    void onImportActionTriggered() { q->importSettings(QString()); }
    void onSettingsActionTriggered() { gCoAPGlobal.showThenMoveToCenter(); }
    void onOpenCachePath()
    {
        QString path = gCoAPGlobal.serverCachePath();
        QDesktopServices::openUrl(path);
    }

private:
    xCoAP* q{nullptr};
};

} // namespace xCoAP
