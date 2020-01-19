/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QUrl>
#include <QDesktopServices>

#include "SAKDownloadItemWidget.hh"

#include "ui_SAKDownloadItemWidget.h"

SAKDownloadItemWidget::SAKDownloadItemWidget(QString downloadUrl, QWidget *parent)
    :QWidget (parent)
    ,ui (new Ui::SAKDownloadItemWidget)
    ,downloadUrl (downloadUrl)
{
    ui->setupUi(this);

    fileNameLabel = ui->fileNameLabel;
    downloadPushButton = ui->downloadPushButton;
    fileNameLabel->setText(downloadUrl);
}

SAKDownloadItemWidget::~SAKDownloadItemWidget()
{

}

void SAKDownloadItemWidget::on_downloadPushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(downloadUrl));
}
