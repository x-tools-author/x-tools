#ifndef GETPUBLICIPTHREAD_H
#define GETPUBLICIPTHREAD_H

#include <QThread>
#include <QHostAddress>

class GetPublicIpThread : public QThread
{
    Q_OBJECT
public:
    GetPublicIpThread(QObject *parent = Q_NULLPTR);
    ~GetPublicIpThread();
public slots:
    QString getPublicIPDirectly();
    void getPublicIP(){emit need2getPublicIp();}
protected:
    void run();
private:
    const QString pageUrl = QString("http://2017.ip138.com/ic.asp");
    const QString backupPageUrl = QString("http://www.3322.org/dyndns/getip");
private slots:
    void getPublicIPActually();
signals:
    void publicIPChanged(QString publicIP);
    void publicIPChanged(QHostAddress publicIP);

    void need2getPublicIp();
};

#endif
