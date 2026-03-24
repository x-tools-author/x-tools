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
#include "xcoap_p.h"

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

#define X_COAP_SETTING_FILE_NAME \
    getLastOpenOrSaveFilePath(d->m_settings) + QString("/xCoapSettings.json")

namespace xCoAP {

struct xCoAPParameterKeys
{
    const QString clientViewVisible{"clientViewVisible"};
    const QString serverViewVisible{"serverViewVisible"};
    const QString client{"client"};
    const QString clientTabIndex{"clientTabIndex"};

    const QString server{"server"};
    const QString serverTabIndex{"serverTabIndex"};

    const QString globalSettings{"globalSettings"};
};

struct xCoAPSettingKeys
{
    const QString openOrSaveFilePath{"openOrSaveFilePath"};
};

xCoAP::xCoAP(QWidget* parent)
    : QWidget(parent)
{
    d = new xCoAPPrivate(this);
    // Client and Server tabs
    d->m_client = new CoAPClientUi(this);
    d->m_server = new CoAPServerUi(this);
    d->ui->tabWidgetClient->addTab(d->m_client, tr("Client", "xCoAP"));
    d->ui->tabWidgetClient->addTab(d->m_client->payloadView(), tr("Payloads", "xCoAP"));
    d->ui->tabWidgetServer->addTab(d->m_server, tr("Server", "xCoAP"));
    d->ui->tabWidgetServer->addTab(d->m_server->resourceView(), tr("Resources", "xCoAP"));
    connect(d->m_client, &CoAPClientUi::invokeChangeTabIndex, this, [=](int index) {
        if (index >= 0 && index < d->ui->tabWidgetClient->count()) {
            d->ui->tabWidgetClient->setCurrentIndex(index);
        }
    });
}

xCoAP::~xCoAP() {}

void xCoAP::setupSettings(QSettings* settings)
{
    d->m_settings = settings;
}

static QString getLastOpenOrSaveFilePath(QSettings* settings)
{
    xCoAPSettingKeys keys;
    QString defaultPath = QDir::homePath();
    if (settings) {
        return settings->value(keys.openOrSaveFilePath, defaultPath).toString();
    } else {
        return defaultPath;
    }
}

static void setLastOpenOrSaveFilePath(QSettings* settings, const QString& fileName)
{
    // Save the directory path for next time use
    if (settings) {
        xCoAPSettingKeys keys;
        QFileInfo fileInfo(fileName);
        QString dirPath = fileInfo.absolutePath();
        settings->setValue(keys.openOrSaveFilePath, dirPath);
    }
}

void xCoAP::exportSettings(const QString& filePath)
{
    QString cookedFilePath = filePath;
    if (cookedFilePath.isEmpty()) {
        cookedFilePath = QFileDialog::getSaveFileName(this,
                                                      tr("Export CoAP Configuration"),
                                                      X_COAP_SETTING_FILE_NAME,
                                                      tr("JSON Files (*.json);;All Files (*)"));
        if (cookedFilePath.isEmpty()) {
            return;
        } else {
            setLastOpenOrSaveFilePath(d->m_settings, cookedFilePath);
        }
    }

    QJsonObject obj = save();
    QFile file(cookedFilePath);
    if (!file.open(QIODevice::WriteOnly)) {
        QMessageBox::warning(this,
                             tr("Export CoAP Configuration Failed"),
                             tr("Failed to open file %1 for writing.").arg(cookedFilePath));
        return;
    }
    QByteArray data = QJsonDocument(obj).toJson(QJsonDocument::Indented);
    file.write(data);
    file.close();
}

void xCoAP::importSettings(const QString& filePath)
{
    QString cookedFilePath = filePath;
    if (cookedFilePath.isEmpty()) {
        cookedFilePath = QFileDialog::getOpenFileName(this,
                                                      tr("Import CoAP Configuration"),
                                                      X_COAP_SETTING_FILE_NAME,
                                                      tr("JSON Files (*.json);;All Files (*)"));
        if (cookedFilePath.isEmpty()) {
            return;
        } else {
            setLastOpenOrSaveFilePath(d->m_settings, cookedFilePath);
        }
    }

    QFile file(cookedFilePath);
    if (!file.open(QIODevice::ReadOnly)) {
        QMessageBox::warning(this,
                             tr("Import CoAP Configuration Failed"),
                             tr("Failed to open file %1 for reading.").arg(cookedFilePath));
        return;
    }
    QByteArray data = file.readAll();
    file.close();

    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson(data, &parseError);
    if (parseError.error != QJsonParseError::NoError) {
        QMessageBox::warning(this,
                             tr("Import CoAP Configuration Failed"),
                             tr("Failed to parse JSON file %1: %2")
                                 .arg(cookedFilePath, parseError.errorString()));
        return;
    }
    if (!doc.isObject()) {
        QMessageBox::warning(this,
                             tr("Import CoAP Configuration Failed"),
                             tr("The content of file %1 is not a valid JSON object.")
                                 .arg(cookedFilePath));
        return;
    }

    load(doc.object());
}

QJsonObject xCoAP::save()
{
    xCoAPParameterKeys keys;
    QJsonObject obj;
    obj.insert(keys.clientViewVisible, d->ui->tabWidgetClient->isVisible());
    obj.insert(keys.client, d->m_client->save());
    obj.insert(keys.clientTabIndex, d->ui->tabWidgetClient->currentIndex());

    obj.insert(keys.serverViewVisible, d->ui->tabWidgetServer->isVisible());
    obj.insert(keys.server, d->m_server->save());
    obj.insert(keys.serverTabIndex, d->ui->tabWidgetServer->currentIndex());

    obj.insert(keys.globalSettings, gCoAPGlobal.save());
    return obj;
}

void xCoAP::load(const QJsonObject& obj)
{
    xCoAPParameterKeys keys;
    bool clientVisible = obj.value(keys.clientViewVisible).toBool(true);
    bool serverVisible = obj.value(keys.serverViewVisible).toBool(true);
    if (!clientVisible && !serverVisible) {
        clientVisible = true;
        serverVisible = true;
    }

    const QString clientCtrlBtnIcon = clientVisible ? ":res/icons/chevron_left.svg"
                                                    : ":res/icons/chevron_right.svg";
    const QString serverCtrlBtnIcon = serverVisible ? ":res/icons/chevron_right.svg"
                                                    : ":res/icons/chevron_left.svg";
    // Client
    d->ui->tabWidgetClient->setVisible(clientVisible);
    d->m_clientCtrlToolButton->setIcon(xIcon(clientCtrlBtnIcon));
    d->m_client->load(obj.value(keys.client).toObject());
    d->ui->tabWidgetClient->setCurrentIndex(obj.value(keys.clientTabIndex).toInt(0));
    // Server
    d->ui->tabWidgetServer->setVisible(serverVisible);
    d->m_serverCtrlToolButton->setIcon(xIcon(serverCtrlBtnIcon));
    d->m_server->load(obj.value(keys.server).toObject());
    d->ui->tabWidgetServer->setCurrentIndex(obj.value(keys.serverTabIndex).toInt(0));
    // Global Settings
    gCoAPGlobal.load(obj.value(keys.globalSettings).toObject());
}

QMenu* xCoAP::toolButtonMenu()
{
    return d->m_toolButtonMenu;
}

} // namespace xCoAP
