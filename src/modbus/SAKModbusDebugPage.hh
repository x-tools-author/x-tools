/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKMODBUSDEBUGGING_HH
#define SAKMODBUSDEBUGGING_HH

#include <QObject>
#include <QWidget>
#include <QSettings>
#include <QSqlDatabase>
#include <QSplashScreen>

namespace Ui {
    class SAKModbusDebugPage;
}

class SAKModbusDebugPage : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKModbusDebugPage(int type, QString name, QSettings *settings = Q_NULLPTR, QSplashScreen *splashScreen = Q_NULLPTR, QSqlDatabase *sqlDatabase = Q_NULLPTR, QWidget *parent = Q_NULLPTR);
    ~SAKModbusDebugPage();

    enum DeviceType {
        TcpClient,
        TcpServer,
        SerialPortClient,
        SerialPortServer
    };
private slots:
    void on_deviceTypeComboBox_currentIndexChanged(int index);

private:
    int mType;
    QString mName;
    QSettings mSettings;
    QSplashScreen *mSplashScreen;
    QSqlDatabase *mSqlDatabase;
private:
    Ui::SAKModbusDebugPage *ui;
};

#endif // SAKMODBUSDEBUGGING_HH
