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
//private:
//    QString mTableName;
//    QTimer mClearMessageTimer;
//private:
//    void readinRecord();
//    bool contains(quint64 paraID);
//    void outputMessage(QString msg, bool isError = false);
//    // update record
//    void changeInterval(int interval);
//    void changeFormat(int format);
//    void changeDescription(QString description);
//    void changeInputText(QString text);
//private:
//    Ui::SAKPluginRegularlySending *mUi;
//    QListWidget *mItemListWidget;
//    QPushButton *mOutportPushButton;
//    QPushButton *mImportPushButton;
//    QPushButton *mDeletePushButton;
//    QPushButton *mAddPushButton;
//    QLabel *mMessageLabel;
//private slots:
//    void on_outportPushButton_clicked();
//    void on_importPushButton_clicked();
//    void on_deletePushButton_clicked();
//    void on_addPushButton_clicked();
};

#endif
