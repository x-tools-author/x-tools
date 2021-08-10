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
    void onBytesRead(const QByteArray &bytes);


protected:
    void insertRecord(const QString &tableName, QWidget *itemWidget) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    quint64 itemId(QWidget *itemWidget) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;


private:
    QSettings *mSettiings;
    QSqlDatabase *mSqlDatabase;
    QSqlQuery mSqlQuery;

    QTimer mClearMessageInfoTimer;
    bool mForbidAll;


private:
    struct SAKStructAutomaticallyResponseJsonKeyContext {
        const QString id = QString("id");
        const QString name = QString("name");
        const QString referenceData = QString("referenceData");
        const QString responseData = QString("responseData");
        const QString enable = QString("enable");
        const QString referenceFormat = QString("referenceFormat");
        const QString responseFormat = QString("responseFormat");
        const QString option = QString("option");
        const QString delay = QString("delay");
        const QString interval = QString("interval");
    };


    struct SAKStructSettingsKeyContext {
        QString disableAutomaticallyResponse;
    }mSettingsKeyCtx;

    struct SAKStructAutomaticallyResponseSqlDatabaseTableContext {
        QString tableName;
        SAKStructAutomaticallyResponseJsonKeyContext columns;
    }mTableCtx;


private:
    void readInRecord();
    void createSqlDatabaseTable();


signals:
    void bytesRead(const QByteArray &);
    void writeBytes(const QByteArray &);
};

#endif
