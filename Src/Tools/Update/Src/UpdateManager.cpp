/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

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
