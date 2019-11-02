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
#ifndef HTTPANALYZER_H
#define HTTPANALTZER_H

#include <QThread>
#include <QUrl>
#include <QNetworkAccessManager>
#include <QNetworkReply>

class HttpAnalyzer : public QThread
{
    Q_OBJECT
public:
    HttpAnalyzer(QUrl configureFile, QString currentVersion = QString("1.0.0"));
    ~HttpAnalyzer();
    void startRequest();
public slots:
    void httpCancel();
private:
    void run();
private:
    /// 服务器端配置文件，该文件保存软件最新版本信息
    QUrl url;
    /// 软件当前版本
    QString curVer;

    QNetworkAccessManager networkMgr;
    QNetworkReply* reply;
    bool httpRequestAborted;
private slots:
    void httpFinished();
signals:
    /**
     * @brief newVersion    --检测到可用更新时，触发该信号
     * @param version       --新版本的版本号
     * @param url           --新版本下载地址
     * @param description   --版本描述
     */
    void newVersion(QString version, QUrl url, QString description);
    void badUrl(QString);
    void noNewVersion();
};

#endif
