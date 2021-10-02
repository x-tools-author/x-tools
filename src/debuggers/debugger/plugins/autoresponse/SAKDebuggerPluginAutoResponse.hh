/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINAUTORESPONSE_HH
#define SAKDEBUGGERPLUGINAUTORESPONSE_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QSqlQuery>
#include <QSettings>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QSqlDatabase>
#include <QListWidgetItem>

#include "SAKBaseListWidget.hh"
#include "SAKDebuggerPluginAutoResponseItem.hh"

class SAKDebuggerPluginAutoResponse : public SAKBaseListWidget
{
   Q_OBJECT
public:
    SAKDebuggerPluginAutoResponse(QSettings *settings,
                                  QString settingsGroup,
                                  QSqlDatabase *sqlDatabase,
                                  QString tableNameSuffix,
                                  QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerPluginAutoResponse();


protected:
    QString sqlCreate(const QString &tableName) final;
    QString sqlInsert(const QString &tableName, QWidget *itemWidget) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    QJsonObject toJsonObject(const QSqlQuery &sqlQuery) final;
    quint64 itemId(QWidget *itemWidget) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;


private:
    struct SAKStructTableContext {
        QString tableName;
        struct {
            const QString id = QString("id");
            const QString description = QString("description");
            const QString referenceData = QString("reference_data");
            const QString responseData = QString("response_data");
            const QString enable = QString("enable");
            const QString referenceFormat = QString("reference_format");
            const QString responseFormat = QString("response_format");
            const QString option = QString("option");
            const QString enableDelay = QString("enable_delay");
            const QString delayTime = QString("delay_time");
        } columns;
    } mTableCtx;

    struct SAKStructSettingsKeyContext {
        QString disableAutomaticallyResponse;
    } mSettingsKeyCtx;
};

#endif
