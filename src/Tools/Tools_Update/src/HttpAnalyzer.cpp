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

#include "HttpAnalyzer.h"
#include <QDebug>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QJsonObject>
#include <QStringList>

HttpAnalyzer::HttpAnalyzer(QUrl configureFile, QString currentVersion)
    :url(configureFile)
    ,curVer(currentVersion)
    ,reply(nullptr)
    ,httpRequestAborted(false)
{
    moveToThread(this);
}

HttpAnalyzer::~HttpAnalyzer()
{

}

void HttpAnalyzer::run()
{
    exec();
}

void HttpAnalyzer::startRequest()
{
    httpRequestAborted = false;
    reply = networkMgr.get(QNetworkRequest(url));
    connect(reply, &QNetworkReply::finished, this, &HttpAnalyzer::httpFinished);
}

void HttpAnalyzer::httpFinished()
{
    /*
     * http将服务器的json配置文件下载至内存，将数据转成json文档，再将文档转为json对象。
     * 从json对象中获取两个值：version和url。
     * version是服务器端最新软件的版本号，url是该版本软件的下载地址。
     * 将当前软件的版本号与服务器端的版本号作比较，判断是否需要更新，如果需要更新，则触发下载信号。
     *
     * 说明：
     *  版本号格式为：主版本号.次版本号.修订版本号
     * json文件内容
     *   {
     *      "version"    :  "1.0.5",
     *      "url"        :  "http://fw.cuav.net/feigong_gs/update.7z"
     *      "description":  "Hello World!"
     *   }
     */
    if (httpRequestAborted){
        reply->deleteLater();
        reply = nullptr;
        return;
    }

    QByteArray json = reply->readAll();
    QJsonParseError error;
    QJsonDocument doc = QJsonDocument::fromJson(json, &error);

    if (error.error != QJsonParseError::NoError){
        emit badUrl(error.errorString());
        reply->deleteLater();
        reply = nullptr;
        return;
    }

    if (doc.isObject()){
        QJsonObject object = doc.object();
        if (!object.isEmpty()){
            QString sVersionStr = object.value("version").toString().trimmed();
            QStringList sVersionList = sVersionStr.split('.');
            if (sVersionList.length() == 3){
                int sVersion = QString("%1%2%3").arg(sVersionList.at(0), 3, '0').arg(sVersionList.at(1), 3, '0').arg(sVersionList.at(2), 3, '0').toInt();
                QString lVersionStr = curVer;
                QStringList lVersionList = lVersionStr.split('.');

                if (lVersionList.length() == 3){
                    int lVersion = QString("%1%2%3").arg(lVersionList.at(0), 3, '0').arg(lVersionList.at(1), 3, '0').arg(lVersionList.at(2), 3, '0').toInt();
                    if (sVersion > lVersion){
                        QUrl url = QUrl(object.value("url").toString());
                        QString description = object.value("description").toString();
                        emit newVersion(sVersionStr, url, description);
                    }else {
                        emit noNewVersion();
                    }
                }else {
                    /// 以下错误断言仅针对开发者
                    Q_ASSERT_X(0, __FUNCTION__, "本地版本号格式有误，正确版本号格式为 主版本号.次版本号.修订版本号；如1.0.0");
                }
            }else {
                /// 以下错误断言仅针对开发者
                Q_ASSERT_X(0, __FUNCTION__, "服务器版本号格式有误或者配置文件分析错误，正确版本号格式为 主版本号.次版本号.修订版本号；如1.0.0");
            }
        }
    }

    reply->deleteLater();
    reply = nullptr;
}

void HttpAnalyzer::httpCancel()
{
    httpRequestAborted = true;

    /// 该操作也会粗发finished信号
    if (reply){
        reply->abort();
    }
}
