/*
 * Copyright 2019-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
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
