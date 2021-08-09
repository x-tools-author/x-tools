/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERPLUGINREGULARLYSENDING_HH
#define SAKDEBUGGERPLUGINREGULARLYSENDING_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>

#include "SAKBaseListWidget.hh"
#include "SAKDebuggerPluginRegularlySendingItem.hh"


class SAKDebuggerPluginRegularlySending : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKDebuggerPluginRegularlySending(QSqlDatabase *sqlDatabase,
                                      QSettings *settings,
                                      QString settingsGroup,
                                      QString tableNameSuffix,
                                      QWidget *parent = Q_NULLPTR);
    ~SAKDebuggerPluginRegularlySending();

    struct SAKStructTableContext {
        QString tableName;
        struct {
            const QString id = QString("id");
            const QString interval = QString("interval");
            const QString format = QString("format");
            const QString description = QString("description");
            const QString data = QString("text");
        } columns;
    };


protected:
    void insertRecord(const QString &tableName, QWidget *itemWidget) final;
    void setItemWidget(QListWidgetItem *item, QWidget *itemWidget) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    quint64 itemId(QWidget *itemWidget) final;


private:
    typedef SAKDebuggerPluginRegularlySendingItem SendingItem;
    SAKStructTableContext mTableCtx;


private:
    void readinRecord();
    void createSqlDatabaseTable();

    void changeInterval(quint64 id, int interval);
    void changeFormat(quint64 id, int format);
    void changeDescription(quint64 id, QString description);
    void changeInputText(quint64 id, QString text);
};

#endif
