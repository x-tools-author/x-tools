/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
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
private:
    QUrl url;
    QString curVer;

    CheckForUpdateDialog* checkForUpdateDialog = nullptr;
    ConfirmUpdateDialog* confirmUpdateDialog   = nullptr;
    DownloadDialog* downloadDialog             = nullptr;
    HttpAnalyzer* httpAnalyzer                 = nullptr;
    HttpDownloader* httpDownloader             = nullptr;
private slots:
    void checkForUpdate();
    void showConfirmUpdateDialog(QString version, QUrl url, QString description);
    void showDownloadDialog(QUrl url);
};

#endif
