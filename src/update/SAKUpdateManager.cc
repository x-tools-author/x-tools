/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QApplication>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QDesktopServices>

#include "SAKUpdateManager.hh"
#include "ui_SAKUpdateManager.h"

static const char* checkForUpdateUrl = "https://api.github.com/repos/wuuhii/QtSwissArmyKnife/releases/latest";
SAKUpdateManager::SAKUpdateManager(QWidget *parent)
    :QDialog(parent)
    ,ui (new Ui::SAKUpdateManager)
{
    ui->setupUi(this);
    currentVersionLabel = ui->currentVersionLabel;
    newVersionLabel = ui->newVersionLabel;
    updateProgressLabel = ui->updateProgressLabel;
    updateProgressBar = ui->updateProgressBar;
    noNewVersionTipLabel = ui->noNewVersionTipLabel;
    newVersionCommentsGroupBox = ui->newVersionCommentsGroupBox;
    newVersionCommentsTextBrowser = ui->newVersionCommentsTextBrowser;
    visitWebPushButton = ui->visitWebPushButton;
    backgroundPushButton = ui->backgroundPushButton;
    downloadUpdatePushButton = ui->downloadUpdatePushButton;
    cancleUpdatePushButton = ui->cancleUpdatePushButton;
    checkForUpdatePushButton = ui->checkForUpdatePushButton;
    infoLabel = ui->infoLabel;

    currentVersionLabel->setText(QApplication::applicationVersion());
    noNewVersionTipLabel->hide();
    visitWebPushButton->setEnabled(false);
    downloadUpdatePushButton->setEnabled(false);

    clearInfoTimer.setInterval(5*1000);
    connect(&clearInfoTimer, &QTimer::timeout, this, &SAKUpdateManager::clearInfo);

    setModal(true);
}

SAKUpdateManager::~SAKUpdateManager()
{

}

void SAKUpdateManager::on_autoCheckForUpdateCheckBox_clicked()
{

}

void SAKUpdateManager::on_visitWebPushButton_clicked()
{
    QDesktopServices::openUrl(QUrl(updateInfo.htmlUrl));
}

void SAKUpdateManager::on_backgroundPushButton_clicked()
{
    close();
}

void SAKUpdateManager::on_downloadUpdatePushButton_clicked()
{

}

void SAKUpdateManager::on_cancleUpdatePushButton_clicked()
{
    close();
}

void SAKUpdateManager::on_checkForUpdatePushButton_clicked()
{
    updateProgressBar->setMaximum(0);
    updateProgressBar->setMaximum(0);
    noNewVersionTipLabel->hide();
    checkForUpdatePushButton->setEnabled(false);

    networkReply = networkAccessManager.get(QNetworkRequest(QUrl(checkForUpdateUrl)));
    connect(networkReply, &QNetworkReply::finished, this, &SAKUpdateManager::checkForUpdateFinished);
}

void SAKUpdateManager::outputInfo(QString info, bool isError)
{
    if (isError){
        info = QString("<font color=red>%1</font>").arg(info);
    }else{
        info = QString("<font color=blue>%1</font>").arg(info);
    }

    infoLabel->setText(info);
    clearInfoTimer.start();
}

void SAKUpdateManager::clearInfo()
{
    clearInfoTimer.stop();
    infoLabel->clear();
}

void SAKUpdateManager::checkForUpdateFinished()
{
    if (networkReply){
        if (networkReply->error() == QNetworkReply::NoError){
            QByteArray data = networkReply->readAll();
            updateInfo = extractUpdateInfo(data);
            if (updateInfo.isValid){
                if (isNewVersion(updateInfo.name)){
                    newVersionLabel->setText(updateInfo.name.remove("v"));
                    newVersionCommentsTextBrowser->setText(updateInfo.body.replace(QString("\\r\\n"), QString("\r\n")));

                    visitWebPushButton->setEnabled(true);
                    downloadUpdatePushButton->setEnabled(true);
                }else{
                    noNewVersionTipLabel->show();
                }

                QApplication::beep();
                updateProgressBar->setMinimum(0);
                updateProgressBar->setMaximum(100);
                updateProgressBar->setValue(100);
            }else{
                outputInfo(updateInfo.errorString, true);
            }
        }else{
            QApplication::beep();            
            outputInfo(networkReply->errorString(), true);
        }
    }

    checkForUpdatePushButton->setEnabled(true);

    delete networkReply;
    networkReply = nullptr;
}

SAKUpdateManager::UpdateInfo SAKUpdateManager::extractUpdateInfo(QByteArray jsonObjectData)
{
    UpdateInfo updateInfo;
    updateInfo.isValid = false;

    if (jsonObjectData.isEmpty()){
        updateInfo.errorString = tr("无法获取更新信息或者无可用发布版本");
    }else{
        QJsonParseError jsonParseError;
        QJsonDocument jsonDoc = QJsonDocument::fromJson(jsonObjectData, &jsonParseError);

        /*
         * 数据格式可查看：resources/files/GitHubLatestReleasesDatastruct.json
         */
        if (jsonParseError.error == QJsonParseError::NoError){
            updateInfo.isValid = true;
            QJsonObject jsonObj = jsonDoc.toVariant().toJsonObject();

            updateInfo.htmlUrl = jsonObj.value("html_url").toString();
            updateInfo.name = jsonObj.value("name").toString();
            updateInfo.browserDownloadUrl  = extractBrowserDownloadUrl(jsonObj.value("assets").toVariant().toJsonArray());
            updateInfo.body = jsonObj.value("body").toString();
        }else{
            updateInfo.errorString = jsonParseError.errorString();
        }
    }

    return updateInfo;
}

QStringList SAKUpdateManager::extractBrowserDownloadUrl(QJsonArray jsonArray)
{
    QStringList urlList;

    for(int i = 0; i < jsonArray.count(); i++){
        QJsonObject jsonObj = jsonArray.at(i).toVariant().toJsonObject();
        QString url = jsonObj.value("browser_download_url").toString();
        urlList.append(url);
    }

    return urlList;
}

bool SAKUpdateManager::isNewVersion(QString remoteVersion)
{
    auto versionStringToInt = [](QString versionString)->int{
        QStringList list = versionString.remove("v").split('.');

        int major = 0;
        int minor = 0;
        int patch = 0;
        if (list.count() == 3){
            major = list.at(0).toInt();
            minor = list.at(1).toInt();
            patch = list.at(2).toInt();
        }

        int ver = 0;
        ver = (major << 16) | (minor << 8) | patch;
        return ver;
    };

    QString localVersion = QApplication::applicationVersion();
    int remoteVer = versionStringToInt(remoteVersion);
    int localVer = versionStringToInt(localVersion);

    if (remoteVer > localVer){
        return true;
    }else{
        return false;
    }
}
