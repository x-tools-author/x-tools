/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGERINPUTDATAPRESET_H
#define SAKDEBUGGERINPUTDATAPRESET_H

#include <QMenu>
#include <QLabel>
#include <QTimer>
#include <QDialog>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QListWidgetItem>

#include "SAKBaseListWidget.hh"
#include "SAKDebuggerInputDataPresetItem.hh"

class SAKDebuggerInputDataPreset : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKDebuggerInputDataPreset(QSqlDatabase *sqlDatabase,
                               QSettings *settings,
                               QString settingsGroup,
                               QMenu *itemsMenu,
                               QWidget *parent = Q_NULLPTR);


public:
    struct SAKStructDataPresetItemTableContext {
        QString tableName;
        struct Columns {
            QString id;
            QString format;
            QString description;
            QString text;
        } columns;

        SAKStructDataPresetItemTableContext() {
            columns.id = QString("ID");
            columns.format = QString("Format");
            columns.description = QString("Description");
            columns.text = QString("Text");
        }
    };


protected:
    QString sqlCreate(const QString &tableName) final;
    QString sqlInsert(const QString &tableName, QWidget *itemWidget) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    QJsonObject toJsonObject(const QSqlQuery &sqlQuery) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    quint64 itemId(QWidget *itemWidget) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;


private:
    QMenu *mItemsMenu;
    SAKStructDataPresetItemTableContext mTableContext;


private:
    void updateFormat(quint64 id, int format);
    void updateDescription(quint64 id, const QString &description);
    void updateText(quint64 id, const QString &text);
signals:
    void invokeWriteCookedBytes(QString rawData, int format);
};

#endif
