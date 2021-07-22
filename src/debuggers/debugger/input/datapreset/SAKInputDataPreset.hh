/******************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#ifndef SAKINPUTDATAPRESET_HH
#define SAKINPUTDATAPRESET_HH

#include <QMenu>
#include <QLabel>
#include <QTimer>
#include <QSettings>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QListWidgetItem>

namespace Ui {
    class SAKInputDataPreset;
}

class SAKInputDataPresetItem;
class SAKInputDataPreset : public QWidget
{
    Q_OBJECT
public:
    SAKInputDataPreset(QSqlDatabase *sqlDatabase,
                       QSettings *settings,
                       QString settingsGroup,
                       QMenu *itemsMenu,
                       QWidget *parent = Q_NULLPTR);
    ~SAKInputDataPreset();
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
private:
    QString mTableName;
    QTimer mClearMessageInfoTimer;
    QSqlDatabase *mSqlDatabase;
    QSettings *mSettings;
    QString mSettingsGroup;
    QMenu *mItemsMenu;
    QSqlQuery mSqlQuery;
    SAKStructDataPresetItemTableContext mTableContext;
private:
    void readinRecord();
    void outputMessage(QString msg, bool isError = false);
    void updateFormat(quint64 id, int format);
    void updateDescription(quint64 id, const QString &description);
    void updateText(quint64 id, const QString &text);
    void insertRecord(const QString &tableName,
                      SAKInputDataPresetItem *itemWidget);
    void setItemWidget(QListWidgetItem *item,
                       SAKInputDataPresetItem *itemWidget);
    void deleteItem(QListWidgetItem *item);
    void exportItems();
    void importItems();
private:
    Ui::SAKInputDataPreset *mUi;
signals:
    void invokeWriteBytes(QString rawData, int format);
};

#endif
