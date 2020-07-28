/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKOTHERTIMINGSENTITEMMANAGER_HH
#define SAKOTHERTIMINGSENTITEMMANAGER_HH

#include <QTimer>
#include <QLabel>
#include <QRegExp>
#include <QWidget>
#include <QLineEdit>
#include <QComboBox>
#include <QCheckBox>
#include <QListWidget>
#include <QPushButton>
#include <QRegExpValidator>

namespace Ui {
    class SAKOtherTimingSentItemManager;
}

class SAKDebugPage;
class SAKOtherTimingSentItemManager:public QWidget
{
    Q_OBJECT
public:
    SAKOtherTimingSentItemManager(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherTimingSentItemManager();

    struct TimingSendingItemKey {
        const QString id = QString("id");
        const QString interval = QString("interval");
        const QString format = QString("format");
        const QString comment = QString("comment");
        const QString data = QString("data");
    };
private:
    SAKDebugPage *debugPage;
    QString tableName;
    QTimer clearMessageTimer;
private:
    void readinRecord();
    bool contains(quint64 paraID);
    void outputMessage(QString msg, bool isError = false);
private:
    Ui::SAKOtherTimingSentItemManager *ui;
    QListWidget *itemListWidget;
    QPushButton *outportPushButton;
    QPushButton *importPushButton;
    QPushButton *deletePushButton;
    QPushButton *addPushButton;
    QLabel *messageLabel;
private slots:
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
