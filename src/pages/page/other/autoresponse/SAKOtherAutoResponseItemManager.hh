/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
    class SAKOtherAutoResponseItemManager;
}

class SAKDebugPage;
/// @brief Auto response item manager
class SAKOtherAutoResponseItemManager:public QWidget
{
   Q_OBJECT
public:
    SAKOtherAutoResponseItemManager(SAKDebugPage *debugPage, QWidget *parent = Q_NULLPTR);
    ~SAKOtherAutoResponseItemManager();

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
private:
    void outputMessage(QString msg, bool isInfo);
    void clearMessage();
    void readInRecord();
    bool contains(quint64 paraID);
private:
    Ui::SAKOtherAutoResponseItemManager *ui;
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
