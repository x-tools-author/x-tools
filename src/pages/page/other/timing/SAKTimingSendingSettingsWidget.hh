/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKTIMINGSENDINGSETTINGSWIDGET_HH
#define SAKTIMINGSENDINGSETTINGSWIDGET_HH

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
    class SAKTimingSendingSettingsWidget;
}

class SAKDebugPage;
class SAKDebugPageDatabaseInterface;
class SAKTimingSendingSettingsWidget:public QWidget
{
    Q_OBJECT
public:
    SAKTimingSendingSettingsWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKTimingSendingSettingsWidget();

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
    SAKDebugPageDatabaseInterface *databaseInterface;
    QTimer clearMessageTimer;
private:
    void readinRecord();
    bool contains(quint64 paraID);
    void outputMessage(QString msg, bool isError = false);
private:
    Ui::SAKTimingSendingSettingsWidget *ui;
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
