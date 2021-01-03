/*
 * Copyright 2019-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKUPDATEMANAGER_HH
#define SAKUPDATEMANAGER_HH

#include <QTimer>
#include <QLabel>
#include <QDialog>
#include <QGroupBox>
#include <QCheckBox>
#include <QSettings>
#include <QListWidget>
#include <QPushButton>
#include <QProgressBar>
#include <QNetworkReply>
#include <QNetworkAccessManager>

namespace Ui {
    class SAKUpdateManager;
}

/// @brief Check for update
class SAKUpdateManager:public QDialog
{
    Q_OBJECT
public:
    SAKUpdateManager(QWidget *parent =  Q_NULLPTR);
    ~SAKUpdateManager();
private:
    struct UpdateInfo{
        bool isValid; // true: the information is valid, false: the information is invalid
        QString errorString; // error description

        QString htmlUrl; // url of release site
        QString name; // verison name, such as 1.0.0
        QStringList browserDownloadUrl; // update packet url
        QString body; // release description
        QString tarballUrl; // source packet(tar.tz)
        QString zipballUrl; // source packet(zip)
    }mUpdateInfo;

    QTimer mClearInfoTimer;
    QNetworkAccessManager mNetworkAccessManager;
    QNetworkReply *mNetworkReply;
    QSettings *mSettings;
    const QString mSettingsKeyUpdateAutomaticallyEnable;
private:
    void outputInfo(QString info, bool isError = false);
    void clearInfo();
    void checkForUpdateFinished();

    UpdateInfo extractUpdateInfo(QByteArray jsonObjectData);
    QStringList extractBrowserDownloadUrl(QJsonArray jsonArray);
    bool isNewVersion(QString remoteVersion);
    void setupDownloadList(UpdateInfo info);
    void clearDownloadList();
    void appendPacketItem(UpdateInfo info, QString icon, QString key);
private:
    Ui::SAKUpdateManager *mUi;
    QLabel *mCurrentVersionLabel;
    QLabel *mNewVersionLabel;
    QLabel *mUpdateProgressLabel;
    QProgressBar *mUpdateProgressBar;
    QLabel *mNoNewVersionTipLabel;
    QGroupBox *mNewVersionCommentsGroupBox;
    QListWidget *mDownloadListListWidget;
    QPushButton *mVisitWebPushButton;
    QPushButton *mCheckForUpdatePushButton;
    QLabel *mInfoLabel;
private slots:
    void on_visitWebPushButton_clicked();
    void on_checkForUpdatePushButton_clicked();
};

#endif
