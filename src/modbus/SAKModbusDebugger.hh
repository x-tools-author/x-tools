/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKMODBUSDEBUGGER_HH
#define SAKMODBUSDEBUGGER_HH

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
class SAKModbusDebugger : public QWidget
{
    Q_OBJECT
public:
    Q_INVOKABLE SAKModbusDebugger(QSettings *settings,
                                  const QString settingsGroup,
                                  QSqlDatabase *sqlDatabase = Q_NULLPTR,
                                  QWidget *parent = Q_NULLPTR);
    ~SAKModbusDebugger();

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
    void updateTableWidget();
    void sendReadRequest();
    void sendWriteRequest();
    void updateTableWidgetData(QModbusDataUnit mdu);
    void updateController(int index);
    void connecteToDevice();
    void disconnecteDevice();
    void updateReadWriteButtonsState();
private:
    Ui::SAKModbusDebugPage *ui;
};

#endif // SAKMODBUSDEBUGGING_HH
