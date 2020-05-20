/*
 * Copyright (C) 2019-2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#ifndef SAKUPDATEMANAGER_HH
#define SAKUPDATEMANAGER_HH

#include <QTimer>
#include <QThread>
#include <QNetworkReply>
#include <QNetworkAccessManager>

/// @brief 更新管理类实例，该类是单实例类
class SAKUpdateManager:public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString appVersion READ appVersion CONSTANT)
    Q_PROPERTY(QString remoteVersion READ remoteVersion WRITE setRemoteVersion NOTIFY remoteVersionChanged)
    Q_PROPERTY(QString msgString READ msgString WRITE setMsgString NOTIFY msgStringChanged)
    Q_PROPERTY(QString updateDescription READ updateDescription WRITE setUpdateDescription NOTIFY updateDescriptionChanged)
private:
    SAKUpdateManager(QObject *parent = Q_NULLPTR);
    ~SAKUpdateManager();
public:
    /**
     * @brief checkForUpdate 检查更新入口
     */
    void checkForUpdate();

    /**
     * @brief instance
     * @return 实例指针
     */
    static SAKUpdateManager *instance();
private:
    struct UpdateInfo{
        bool isValid;                   // 是否可用
        QString errorString;            // 错误信息

        QString htmlUrl;                // 发布页面地址
        QString name;                   // 最新版本号
        QStringList browserDownloadUrl; // 下载链接
        QString body;                   // 发布描述
        QString tarballUrl;             // 源码包（tar.tz）
        QString zipballUrl;             // 源码包（zip）
    }updateInfo;

    QNetworkAccessManager networkAccessManager;
    QNetworkReply *networkReply;
    static SAKUpdateManager *instancePtr;
private:
    void checkForUpdateFinished();

    UpdateInfo extractUpdateInfo(QByteArray jsonObjectData);
    QStringList extractBrowserDownloadUrl(QJsonArray jsonArray);
    bool isNewVersion(QString remoteVersion);
private:
    QString _appVersion;
    QString appVersion();

    QString _remoteVersion;
    QString remoteVersion();
    void setRemoteVersion(QString ver);

    QString _msgString;
    QString msgString();
    void setMsgString(QString msg);

    QString _updateDescription;
    QString updateDescription();
    void setUpdateDescription(QString des);
signals:
    void remoteVersionChanged();
    void msgStringChanged();
    void updateDescriptionChanged();
};

#endif
