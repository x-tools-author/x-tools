/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKINPUTDATAPRESET_HH
#define SAKINPUTDATAPRESET_HH

#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
    class SAKInputDataPreset;
}

class SAKDebugger;
class SAKInputDataPresetItem;
class SAKDebugPageCommonDatabaseInterface;
/// @brief Data preset item manager widget
class SAKInputDataPreset:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataPreset(SAKDebugger *mDebugPage, QWidget *parent = Q_NULLPTR);
    ~SAKInputDataPreset();

    struct DataPresetItemContext {
        const QString id = QString("id");
        const QString format = QString("format");
        const QString description = QString("comment");
        const QString text = QString("data");
    };

    /**
     * @brief itemList: Get the item list
     * @return Item list
     */
    QList<SAKInputDataPresetItem*> itemList();
private:
    SAKDebugger *mDebugPage;
    QListWidget *mListWidget;
    QString mTableName;
    QTimer mClearMessageInfoTimer;
    SAKDebugPageCommonDatabaseInterface *mDatabaseInterface;
private:
    void readinRecord();
    void outputMessage(QString msg, bool isError = false);
    bool contains(quint64 paraID);
    void appendDataPresetItem(QWidget *iw);
    // update record
    void updateFormat(int format);
    void updateDescription(const QString &text);
    void updateText(QString text);
private:
    Ui::SAKInputDataPreset *mUi;
    QPushButton *mDeletePushButton;
    QPushButton *mAddPushButton;
    QPushButton *mOutportPushButton;
    QPushButton *mImportPushButton;
    QLabel *mInfoLabel;
private slots:
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
signals:
    void itemDeleted(SAKInputDataPresetItem *item);
    void itemAdded(SAKInputDataPresetItem *item);
    void descriptionChanged(SAKInputDataPresetItem *item);
};

#endif
