/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKTCPTRANSPONDERS_HH
#define SAKTCPTRANSPONDERS_HH

#include "SAKBaseListWidget.hh"

class SAKTcpTransponders : public SAKBaseListWidget
{
    Q_OBJECT
public:
    SAKTcpTransponders(QSqlDatabase *sqlDatabase,
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
            const QString serverHost = QString("serverHost");
            const QString serverPort = QString("serverPort");
        }columns;
    }mTableCtx;
};

#endif // SAKTCPTRANSPONDERS_HH
