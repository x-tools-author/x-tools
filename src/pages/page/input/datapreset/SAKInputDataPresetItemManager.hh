/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKINPUTDATAITEMMANAGER_HH
#define SAKINPUTDATAITEMMANAGER_HH

#include <QLabel>
#include <QTimer>
#include <QCheckBox>
#include <QComboBox>
#include <QLineEdit>
#include <QTextEdit>
#include <QListWidget>
#include <QPushButton>

namespace Ui {
    class SAKInputDataPresetItemManager;
}

class SAKDebugPage;
class SAKDebugPageCommonDatabaseInterface;
/// @brief 预设数据管理窗口
class SAKInputDataPresetItemManager:public QWidget
{
    Q_OBJECT
public:
    SAKInputDataPresetItemManager(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKInputDataPresetItemManager();

    struct DataPresetItemContext {
        const QString id = QString("id");
        const QString format = QString("format");
        const QString description = QString("comment");
        const QString text = QString("data");
    };
private:
    SAKDebugPage *debugPage;
    QListWidget *listWidget;
    QString tableName;
    QTimer clearMessageInfoTimer;
    SAKDebugPageCommonDatabaseInterface *databaseInterface;
private:
    void readinRecord();
    void outputMessage(QString msg, bool isError = false);
    bool contains(quint64 paraID);
    // update record
    void updateFormat(int format);
    void updateDescription(const QString &text);
    void updateText(QString text);
private:
    Ui::SAKInputDataPresetItemManager *ui;
    QPushButton *deletePushButton;
    QPushButton *addPushButton;
    QPushButton *outportPushButton;
    QPushButton *importPushButton;
    QLabel *infoLabel;
private slots:
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
};

#endif
