/**
 * @copybrief   Copyright © 2018, WuHai all rights reserve.
 * @author      wuhai
 * @em          wuhai1024@outlook.com
 * @date        The file is created on Dec. 29, 2018 by wuhai.
 * @brief       The device info.
 * @version
 *      20181229  --v1.0.0 create the file.
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
