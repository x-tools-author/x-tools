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
#include "CheckForUpdateDialog.h"
#include "ConfirmUpdateDialog.h"
#include "DownloadDialog.h"
#include "HttpAnalyzer.h"
#include "HttpDownloader.h"
#include "UpdateManager.h"

UpdateManager::UpdateManager(QUrl configureFile, QString currentVersion)
    :url(configureFile)
    ,curVer(currentVersion)
    ,confirmUpdateDialog(new ConfirmUpdateDialog)
    ,httpDownloader(new HttpDownloader)
{
    confirmUpdateDialog->setCurrentVersion(currentVersion);
    httpAnalyzer = new HttpAnalyzer(url, curVer);
    checkForUpdateDialog = new CheckForUpdateDialog(httpAnalyzer);
    downloadDialog = new DownloadDialog(httpDownloader);

    connect(httpAnalyzer, &HttpAnalyzer::newVersion, this, &UpdateManager::showConfirmUpdateDialog);
    connect(confirmUpdateDialog, &ConfirmUpdateDialog::download, this, &UpdateManager::showDownloadDialog);

    httpAnalyzer->start();
    httpDownloader->start();
}

UpdateManager::~UpdateManager()
{

}

void UpdateManager::checkForUpdate()
{
    checkForUpdateDialog->checkForUpdate();
    checkForUpdateDialog->activateWindow();
}

void UpdateManager::showConfirmUpdateDialog(QString version, QUrl url, QString description)
{
    if (!checkForUpdateDialog->isHidden()){
        checkForUpdateDialog->close();
    }

    confirmUpdateDialog->setInfo(version, url, description);
    confirmUpdateDialog->show();
    confirmUpdateDialog->activateWindow();
}

void UpdateManager::showDownloadDialog(QUrl url)
{
    downloadDialog->downloadFile(url);
}
