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


protected:
    QString sqlCreate(const QString &tableName) final;
    QString sqlInsert(const QString &tableName, QWidget *itemWidget) final;
    QJsonObject toJsonObject(QWidget *itemWidget) final;
    QJsonObject toJsonObject(const QSqlQuery &sqlQuery) final;
    QWidget *createItemFromParameters(const QJsonObject &jsonObj) final;
    quint64 itemId(QWidget *itemWidget) final;
    void connectSignalsToSlots(QWidget *itemWidget) final;


private:
    struct SAKStructTableContext {
        QString tableName;
        struct {
            const QString id = QString("id");
            QString format = QString("format");
            QString description = QString("description");
            QString data = QString("data");
        } columns;
    };


private:
    QMenu *mItemsMenu;
    SAKStructTableContext mTableContext;


private:
    void onDescriptionChanged(quint64 id, const QString &description);
    void onFormatChanged(quint64 id, int format);
    void onDataChanged(quint64 id, const QString &text);


signals:
    void invokeWriteString(QString rawData, int format);
};

#endif
