/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKPLUGINAUTOMATICALLYRESPONSE_HH
#define SAKPLUGINAUTOMATICALLYRESPONSE_HH

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

#include "SAKPluginAutomaticallyResponseItem.hh"

namespace Ui {
    class SAKPluginAutomaticallyResponse;
}

/// @brief Auto response item manager
class SAKPluginAutomaticallyResponse:public QWidget
{
   Q_OBJECT
public:
    SAKPluginAutomaticallyResponse(QSettings *settings,
                                   QString settingsGroup,
                                   QSqlDatabase *sqlDatabase,
                                   QWidget *parent = Q_NULLPTR);
    ~SAKPluginAutomaticallyResponse();
    void onBytesRead(const QByteArray &bytes);
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
    }mSqlDatabaseTableCtx;
private:
    void outputMessage(QString msg, bool isInfo);
    void readInRecord();
    void insertRecord(SAKPluginAutomaticallyResponseItem::ITEM_CTX itemCtx);
    void addItem(SAKPluginAutomaticallyResponseItem::ITEM_CTX itemCtx);
    void exportItems();
    void importItems();
    bool itemIsExisted(quint64 id);
    void deleteItem();
    void addItemWidthoutParameters();
    void createSqlDatabaseTable();
    void clearItems();
    void setItemWidget(QListWidgetItem *item,
                       SAKPluginAutomaticallyResponseItem *itemWidget,
                       QListWidget *listWidget);

private:
    Ui::SAKPluginAutomaticallyResponse *mUi;


signals:
    void bytesRead(const QByteArray &);
    void writeBytes(const QByteArray &);
};

#endif
