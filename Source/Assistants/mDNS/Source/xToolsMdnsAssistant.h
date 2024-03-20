/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QTreeWidgetItem>
#include <QWidget>

#include <qmdnsengine/service.h>

namespace Ui {
class xToolsMdnsAssistant;
}

class xToolsMdnsServer;
class xToolsMdnsAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE xToolsMdnsAssistant(QWidget *parent = nullptr);
    ~xToolsMdnsAssistant();

private:
    struct
    {
        const QString serviceTypes{"serviceTypes"};
        const QString currentServiceType{"currentServiceType"};
    } m_settingKeys;

private:
    Ui::xToolsMdnsAssistant *ui;
    xToolsMdnsServer *m_server;

private:
    void startMdnsService();
    void stopMdnsService();
    void setupItem(const QMdnsEngine::Service &service);
    void setUiState(bool isRunning);
    void loadSettings();
    void saveSettings();
    QTreeWidgetItem *findItem(const QMdnsEngine::Service &service);

    void onServiceAdded(const QMdnsEngine::Service &service);
    void onServiceUpdated(const QMdnsEngine::Service &service);
    void onServiceRemoved(const QMdnsEngine::Service &service);
};
