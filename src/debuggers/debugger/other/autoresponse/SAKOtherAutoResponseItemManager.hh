/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERAUTORESPONSEITEMMANAGER_HH
#define SAKOTHERAUTORESPONSEITEMMANAGER_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
    class SAKOtherAutoResponseItemManager;
}

class SAKDebugPage;
class SAKOtherAutoResponseItem;
class SAKDebugPageCommonDatabaseInterface;
/// @brief Auto response item manager
class SAKOtherAutoResponseItemManager:public QWidget
{
   Q_OBJECT
public:
    SAKOtherAutoResponseItemManager(SAKDebugPage *mDebugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherAutoResponseItemManager();

    struct AutoResponseItemKey {
        const QString id = QString("id");
        const QString description = QString("description");
        const QString referenceText = QString("referenceText");
        const QString responseText = QString("responseText");
        const QString enable = QString("enable");
        const QString referenceFormat = QString("referenceFormat");
        const QString responseFormat = QString("responseFormat");
        const QString option = QString("option");
        const QString delay = QString("delay");
        const QString interval = QString("interval");
    };
private:
    QTimer mClearMessageInfoTimer;
    SAKDebugPage *mDebugPage;
    SAKDebugPageCommonDatabaseInterface *mDatabaseInterface;
    QString mTableName;
private:
    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
    void readInRecord();
    bool contains(quint64 paraID);
    void initializingItem(SAKOtherAutoResponseItem *item);
    // update record
    void changeDescription(const QString &description);
    void changeReferenceText(const QString &text);
    void changeResponseText(const QString &text);
    void changeEnable(bool enable);
    void changeOption(int option);
    void changeReferenceFormat(int format);
    void changeResponseFromat(int format);
    void changeDelay(bool delay);
    void changeInterval(int interval);
    SAKOtherAutoResponseItem *sender2item(QObject *sender);
    QList<SAKOtherAutoResponseItem *> items();
private:
    Ui::SAKOtherAutoResponseItemManager *mUi;
    QListWidget *mListWidget;
    QCheckBox *mForbidAllCheckBox;
    QPushButton *mDeleteItemPushButton;
    QPushButton *mAddItemPushButton;
    QPushButton *mOutportPushButton;
    QPushButton *mImportPushButton;
    QLabel *mMsgLabel;
private slots:
    void on_forbidAllCheckBox_clicked();
    void on_deleteItemPushButton_clicked();
    void on_addItemPushButton_clicked();
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
};

#endif
