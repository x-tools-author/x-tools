/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKAUTORESPONSESETTINGSWIDGET_HH
#define SAKAUTORESPONSESETTINGSWIDGET_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

namespace Ui {
    class SAKAutoResponseSettingsWidget;
}

class SAKDebugPage;
class SAKDebugPageDatabaseInterface;
/// @brief 自动回复设置窗口
class SAKAutoResponseSettingsWidget:public QWidget
{
   Q_OBJECT
public:
    SAKAutoResponseSettingsWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKAutoResponseSettingsWidget();

    struct AutoResponseItemKey {
        const QString id = QString("id");
        const QString name = QString("name");
        const QString referenceData = QString("referenceData");
        const QString responseData = QString("responseData");
        const QString enable = QString("enable");
        const QString referenceFormat = QString("referenceFormat");
        const QString responseFormat = QString("responseFormat");
        const QString option = QString("option");
    };
private:
    QTimer clearMessageInfoTimer;
    SAKDebugPage *debugPage;
    SAKDebugPageDatabaseInterface *databaseInterface;
private:
    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
    void readInRecord();
    bool contains(quint64 paraID);
private:
    Ui::SAKAutoResponseSettingsWidget *ui;
    QListWidget *listWidget;
    QCheckBox *forbidAllCheckBox;
    QPushButton *deleteItemPushButton;
    QPushButton *addItemPushButton;
    QPushButton *outportPushButton;
    QPushButton *importPushButton;
    QLabel *msgLabel;
private slots:
    void on_forbidAllCheckBox_clicked();
    void on_deleteItemPushButton_clicked();
    void on_addItemPushButton_clicked();
    void on_outportPushButton_clicked();
    void on_importPushButton_clicked();
};

#endif
