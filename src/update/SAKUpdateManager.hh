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
#ifndef SAKUPDATEMANAGER_HH
#define SAKUPDATEMANAGER_HH

#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QListView>
#include <QGroupBox>
#include <QPushButton>
#include <QProgressBar>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace Ui {
    class SAKUpdateManager;
}

class SAKUpdateManager:public QDialog
{
    Q_OBJECT
public:
    SAKUpdateManager(QWidget *parent =  nullptr);
    ~SAKUpdateManager();
private:
    Ui::SAKUpdateManager *ui;
    QLabel *currentVersionabel;
    QLabel *updateProgressLabel;
    QProgressBar *updateProgressBar;
    QLabel *noNewVersionTipLabel;
    QGroupBox *newVersionCommentsGroupBox;
    QListView *newVersionCommentsListView;
    QPushButton *visitWebPushButton;
    QPushButton *backgroundPushButton;
    QPushButton *downloadUpdatePushButton;
    QPushButton *cancleUpdatePushButton;
    QPushButton *checkForUpdatePushButton;
    QLabel *infoLabel;

private slots:
    void on_visitWebPushButton_clicked();
    void on_backgroundPushButton_clicked();
    void on_downloadUpdatePushButton_clicked();
    void on_cancleUpdatePushButton_clicked();
    void on_checkForUpdatePushButton_clicked();

private:
    QTimer clearInfoTimer;
    QUrl checkForUpdateUrl;
    QNetworkAccessManager networkAccessManager;
    QNetworkReply *networkReply;

private:
    void outputInfo(QString info, bool isError = false);
    void clearInfo();
    void checkForUpdateFinished();
};

#endif
