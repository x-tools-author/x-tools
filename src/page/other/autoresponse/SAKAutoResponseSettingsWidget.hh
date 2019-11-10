/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef AUTORESPONSESETTINGWIDGET_HH
#define AUTORESPONSESETTINGWIDGET_HH

#include <QTimer>
#include <QLabel>
#include <QWidget>
#include <QCheckBox>
#include <QLineEdit>
#include <QPushButton>
#include <QListWidget>
#include <QListWidgetItem>

class SAKDebugPage;

namespace Ui {
    class AutoResponseSettingWidget;
}

class AutoResponseSettingWidget:public QWidget
{
   Q_OBJECT
public:
    AutoResponseSettingWidget(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~AutoResponseSettingWidget();
private:
    SAKDebugPage    *_debugPage;
    QListWidget     *listWidget;
    QCheckBox       *forbidAllCheckBox;
    QPushButton     *deleteItemPushButton;
    QPushButton     *addItemPushButton;
    QLabel          *msgLabel;
private slots:
    void on_forbidAllCheckBox_clicked();
    void on_deleteItemPushButton_clicked();
    void on_addItemPushButton_clicked();
private:
    QTimer clearMessageInfoTimer;

    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
private:
    Ui::AutoResponseSettingWidget *ui;
};

#endif
