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
#include <QDebug>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>
#include <QNetworkRequest>
#include <QDesktopServices>
#include <QGuiApplication>

#include "SAKUpdateManager.hh"

static const char* checkForUpdateUrl = "https://api.github.com/repos/wuuhii/QtSwissArmyKnife/releases/latest";
SAKUpdateManager *SAKUpdateManager::instancePtr = Q_NULLPTR;
SAKUpdateManager::SAKUpdateManager(QObject *parent)
    :QObject(parent)
{
    instancePtr = this;

    _appVersion = QCoreApplication::applicationVersion();
    _remoteVersion = QString("0.0.0");
}

SAKUpdateManager::~SAKUpdateManager()
{

}

void SAKUpdateManager::checkForUpdate()
{
    networkReply = networkAccessManager.get(QNetworkRequest(QUrl(checkForUpdateUrl)));
    connect(networkReply, &QNetworkReply::finished,
            this, &SAKUpdateManager::checkForUpdateFinished,
            static_cast<Qt::ConnectionType>(Qt::AutoConnection|Qt::UniqueConnection));
}

SAKUpdateManager *SAKUpdateManager::instance()
{
    if (!instancePtr){
        new SAKUpdateManager;
    }

    Q_ASSERT_X(instancePtr, __FUNCTION__, "Oh, a null pointer");
    return instancePtr;
}

void SAKUpdateManager::checkForUpdateFinished()
{
    if (networkReply){
        if (networkReply->error() == QNetworkReply::NoError){
            QByteArray data = networkReply->readAll();
            updateInfo = extractUpdateInfo(data);
            if (updateInfo.isValid){
                if (isNewVersion(updateInfo.name)){
                    setRemoteVersion(updateInfo.name.remove("v"));
                    setUpdateDescription(updateInfo.body.replace(QString("\\r\\n"), QString("\r\n")));
                }else{
                    setRemoteVersion(updateInfo.name.remove("v"));
                }
            }else{
                setMsgString(updateInfo.errorString);
            }
        }else{
            setMsgString(networkReply->errorString());
        }
    }

    delete networkReply;
    networkReply = Q_NULLPTR;
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

        /// @brief 数据格式可查看：resources/files/GitHubLatestReleasesDatastruct.json
        if (jsonParseError.error == QJsonParseError::NoError){
            updateInfo.isValid = true;
            QJsonObject jsonObj = jsonDoc.toVariant().toJsonObject();

            updateInfo.htmlUrl = jsonObj.value("html_url").toString();
            updateInfo.name = jsonObj.value("name").toString();
            updateInfo.browserDownloadUrl  = extractBrowserDownloadUrl(jsonObj.value("assets").toVariant().toJsonArray());
            updateInfo.body = jsonObj.value("body").toString();
            updateInfo.tarballUrl = jsonObj.value("tarball_url").toString();
            updateInfo.zipballUrl = jsonObj.value("zipball_url").toString();
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

    QString localVersion = _appVersion;
    int remoteVer = versionStringToInt(remoteVersion);
    int localVer = versionStringToInt(localVersion);

    if (remoteVer > localVer){
        return true;
    }else{
        return false;
    }
}

QString SAKUpdateManager::appVersion()
{
    return _appVersion;
}

QString SAKUpdateManager::remoteVersion()
{
    return _remoteVersion;
}

void SAKUpdateManager::setRemoteVersion(QString ver)
{
    _remoteVersion = ver;
    emit remoteVersionChanged();
}

QString SAKUpdateManager::msgString()
{
    return _msgString;
}

void SAKUpdateManager::setMsgString(QString msg)
{
    _msgString = msg;
    emit msgStringChanged();
}

QString SAKUpdateManager::updateDescription()
{
    return _updateDescription;
}

void SAKUpdateManager::setUpdateDescription(QString des)
{
    _updateDescription = des;
    emit updateDescriptionChanged();
}
