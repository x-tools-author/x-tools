/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINTIMEDSENDING_HH
#define SAKDEBUGGERPLUGINTIMEDSENDING_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>

#include "SAKBaseListWidget.hh"
#include "SAKDebuggerPluginTimedSendingItem.hh"


class SAKDebuggerPluginTimedSending : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKDebuggerPluginTimedSending(QSqlDatabase *sqlDatabase,
                                  QSettings *settings,
                                  QString settingsGroup,
                                  QString tableNameSuffix,
                                  QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerPluginTimedSending();

    struct SAKStructTableContext {
        QString tableName;
        struct {
            const QString id = QString("id");
            const QString enable = QString("enable");
            const QString description = QString("description");
            const QString interval = QString("interval");
            const QString format = QString("format");
            const QString suffix = QString("suffix");
            const QString data = QString("data");
        } columns;
    };


protected:
    QString sqlInsert(const QString &tableName, QWidget *itemWidget) final;
    QString sqlCreate(const QString &tableName) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    QJsonObject toJsonObject(const QSqlQuery &sqlQuery) final;
    quint64 itemId(QWidget *itemWidget) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;


private:
    typedef SAKDebuggerPluginTimedSendingItem SendingItem;
    SAKStructTableContext mTableCtx;


private:
    void onEnableChanged(quint64 id, bool enable);
    void onDescriptionChanged(quint64 id, QString description);
    void onIntervalChanged(quint64 id, int interval);
    void onFormatChanged(quint64 id, int format);
    void onSuffixChanged(quint64 id, int format);
    void onDataChanged(quint64 id, QString text);
};

#endif
