/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKMDNSASSISTANT_H
#define SAKMDNSASSISTANT_H

#include <QTreeWidgetItem>
#include <QWidget>

#include <qmdnsengine/service.h>

namespace Ui {
class SAKMdnsAssistant;
}

class SAKMdnsServer;
class SAKMdnsAssistant : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKMdnsAssistant(QWidget *parent = nullptr);
    ~SAKMdnsAssistant();

private:
    struct
    {
        const QString serviceTypes{"serviceTypes"};
        const QString currentServiceType{"currentServiceType"};
    } m_settingKeys;

private:
    Ui::SAKMdnsAssistant *ui;
    SAKMdnsServer *m_server;

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

#endif // SAKMDNSASSISTANT_H
