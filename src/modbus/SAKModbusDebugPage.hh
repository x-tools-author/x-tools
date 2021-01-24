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
#include <QPushButton>
#include <QSqlDatabase>
#include <QSplashScreen>
#include <QModbusDataUnit>

namespace Ui {
    class SAKModbusDebugPage;
}

class SAKModbusCommonController;
class SAKModbusCommonRegisterView;
class SAKModbusCommonRegisterViewController;
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

    struct SettingsKeyContext {
        QString pageIndex;
    };
private:
    int mType;
    QString mName;
    QSettings mSettings;
    QSplashScreen *mSplashScreen;
    QSqlDatabase *mSqlDatabase;
    QPushButton *mMenuPushButton;
    SAKModbusCommonController *mController;
    QList<SAKModbusCommonRegisterView *> mRegisterViewList;
    QList<SAKModbusCommonRegisterViewController *> mRegisterViewControllerList;
    SettingsKeyContext mSettingsKeyContext;
private:
    QWidget *controllerFromType(int type);
    void outputModbusDataUnit(QModbusDataUnit mdu);
    void setData(QModbusDataUnit::RegisterType type, quint16 address, quint16 value);
    void updateRegisterValue(QModbusDataUnit::RegisterType registerTyp, quint16 startAddress, quint16 addressNumber);
    SAKModbusCommonRegisterView *registerView(QModbusDataUnit::RegisterType registerTyp);
    // Just for modbus server
    void dataWritten(QModbusDataUnit::RegisterType table, int address, int size);
    void outputMessage(QString msg, bool isErrorMsg = false);
private:
    Ui::SAKModbusDebugPage *ui;
private slots:
    void on_deviceTypeComboBox_currentIndexChanged(int index);
    void on_connectionPushButton_clicked();
    void on_disconnectionPushButton_clicked();
};

#endif // SAKMODBUSDEBUGGING_HH
