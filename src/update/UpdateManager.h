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
#ifndef UPDATEMANAGER_H
#define UPDATEMANAGER_H

#include <QDialog>
#include <QUrl>

class CheckForUpdateDialog;
class ConfirmUpdateDialog;
class DownloadDialog;
class HttpAnalyzer;
class HttpDownloader;

class UpdateManager:public QDialog
{
    Q_OBJECT
public:
    UpdateManager(QUrl configureFile, QString currentVersion = QString("1.0.0"));
    ~UpdateManager();

    void checkForUpdate();
private:
    QUrl url;
    QString curVer;

    CheckForUpdateDialog* checkForUpdateDialog = nullptr;
    ConfirmUpdateDialog* confirmUpdateDialog   = nullptr;
    DownloadDialog* downloadDialog             = nullptr;
    HttpAnalyzer* httpAnalyzer                 = nullptr;
    HttpDownloader* httpDownloader             = nullptr;
private slots:    
    void showConfirmUpdateDialog(QString version, QUrl url, QString description);
    void showDownloadDialog(QUrl url);
};

#endif
