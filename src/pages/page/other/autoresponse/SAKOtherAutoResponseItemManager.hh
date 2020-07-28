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
        const QString referenceData = QString("referenceData");
        const QString responseData = QString("responseData");
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
private:
    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
    void readInRecord();
    bool contains(quint64 paraID);
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
