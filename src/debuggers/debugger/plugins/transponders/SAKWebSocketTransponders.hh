/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKWEBSOCKETTRANSPONDERS_HH
#define SAKWEBSOCKETTRANSPONDERS_HH

#include "SAKBaseListWidget.hh"

class SAKWebSocketTransponders : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKWebSocketTransponders(QSqlDatabase *sqlDatabase,
                             QSettings *settings,
                             QString settingsGroup,
                             QString tableNameSuffix,
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
            const QString serverAddress = QString("serverAddress");
            const QString sendingType = QString("sendingType");
        }columns;
    }mTableCtx;
};

#endif // SAKWEBSOCKETTRANSPONDERS_HH
