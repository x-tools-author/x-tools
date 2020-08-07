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
    :QWidget(parent)
    ,mDownloadUrl(downloadUrl)
    ,mUi(new Ui::SAKDownloadItemWidget)
{
    mUi->setupUi(this);

    // Initializing variables
    mFileNameLabel = mUi->fileNameLabel;
    mDownloadPushButton = mUi->downloadPushButton;
    mFileNameLabel->setText(downloadUrl);
}

SAKDownloadItemWidget::~SAKDownloadItemWidget()
{
    delete mUi;
}

void SAKDownloadItemWidget::on_downloadPushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(mDownloadUrl));
}
