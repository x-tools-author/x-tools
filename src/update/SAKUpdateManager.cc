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
#include <QApplication>
#include <QNetworkRequest>

#include "SAKUpdateManager.hh"
#include "ui_SAKUpdateManager.h"

SAKUpdateManager::SAKUpdateManager(QWidget *parent)
    :QDialog(parent)
    ,ui (new Ui::SAKUpdateManager)
{
    ui->setupUi(this);
    currentVersionabel = ui->currentVersionabel;
    updateProgressLabel = ui->updateProgressLabel;
    updateProgressBar = ui->updateProgressBar;
    noNewVersionTipLabel = ui->noNewVersionTipLabel;
    newVersionCommentsGroupBox = ui->newVersionCommentsGroupBox;
    newVersionCommentsListView = ui->newVersionCommentsListView;
    visitWebPushButton = ui->visitWebPushButton;
    backgroundPushButton = ui->backgroundPushButton;
    downloadUpdatePushButton = ui->downloadUpdatePushButton;
    cancleUpdatePushButton = ui->cancleUpdatePushButton;
    checkForUpdatePushButton = ui->checkForUpdatePushButton;
    infoLabel = ui->infoLabel;

    currentVersionabel->setText(QApplication::applicationVersion());
    noNewVersionTipLabel->hide();

    checkForUpdateUrl = QUrl("https://api.github.com/repos/wuuhii/QtSwissArmyKnife/releases/latest");
    clearInfoTimer.setInterval(5*1000);
    connect(&clearInfoTimer, &QTimer::timeout, this, &SAKUpdateManager::clearInfo);

    setModal(true);
}

SAKUpdateManager::~SAKUpdateManager()
{

}

void SAKUpdateManager::on_visitWebPushButton_clicked()
{

}

void SAKUpdateManager::on_backgroundPushButton_clicked()
{

}

void SAKUpdateManager::on_downloadUpdatePushButton_clicked()
{

}

void SAKUpdateManager::on_cancleUpdatePushButton_clicked()
{

}

void SAKUpdateManager::on_checkForUpdatePushButton_clicked()
{
    updateProgressBar->setMaximum(0);
    updateProgressBar->setMaximum(0);
    checkForUpdatePushButton->setEnabled(false);

    networkReply = networkAccessManager.get(QNetworkRequest(checkForUpdateUrl));
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

        }else{
            QApplication::beep();
            QString info = networkReply->errorString();
            outputInfo(info);
            checkForUpdatePushButton->setEnabled(true);

            delete networkReply;
            networkReply = nullptr;
        }
    }
}
