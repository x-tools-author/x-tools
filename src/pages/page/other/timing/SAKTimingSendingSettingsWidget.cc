/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDateTime>

#include "SAKGlobal.hh"
#include "SAKTimingSendingItemWidget.hh"
#include "SAKTimingSendingSettingsWidget.hh"

#include "ui_SAKTimingSendingSettingsWidget.h"

SAKTimingSendingSettingsWidget::SAKTimingSendingSettingsWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKTimingSendingSettingsWidget)
    ,debugPage (debugPage)
{
    ui->setupUi(this);

    itemListWidget      = ui->itemListWidget;
    savePushButton      = ui->savePushButton;
    importPushButton    = ui->importPushButton;
    deletePushButton    = ui->deletePushButton;
    addPushButton       = ui->addPushButton;

    on_addPushButton_clicked();
}

SAKTimingSendingSettingsWidget::~SAKTimingSendingSettingsWidget()
{
    delete ui;
}

void SAKTimingSendingSettingsWidget::on_savePushButton_clicked()
{

}

void SAKTimingSendingSettingsWidget::on_importPushButton_clicked()
{

}

void SAKTimingSendingSettingsWidget::on_deletePushButton_clicked()
{
    QListWidgetItem *currentItem = itemListWidget->currentItem();
    if (currentItem){
        itemListWidget->removeItemWidget(currentItem);
        delete currentItem;
    }
}

void SAKTimingSendingSettingsWidget::on_addPushButton_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(itemListWidget);
    itemListWidget->addItem(item);

    SAKTimingSendingItemWidget *itemWidget = new SAKTimingSendingItemWidget(debugPage);
    item->setSizeHint(itemWidget->size());
    itemListWidget->setItemWidget(item, itemWidget);
}
