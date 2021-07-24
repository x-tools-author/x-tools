/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKDEBUGGER_HH
#define SAKDEBUGGER_HH

#include <QSize>
#include <QTimer>
#include <QLabel>
#include <QMutex>
#include <QWidget>
#include <QCheckBox>
#include <QGroupBox>
#include <QComboBox>
#include <QDateTime>
#include <QMetaEnum>
#include <QTextEdit>
#include <QSettings>
#include <QListWidget>
#include <QGridLayout>
#include <QPushButton>
#include <QHBoxLayout>
#include <QApplication>
#include <QTextBrowser>
#include <QSqlDatabase>

class SAKDebuggerInput;
class SAKDebuggerDevice;
class SAKDebuggerOutput;
class SAKDebuggerPlugins;
class SAKDebuggerStatistics;
class SAKCommonCrcInterface;
class SAKDebuggerController;
class SAKPluginDataForwarding;
class SAKDebugPageCommonDatabaseInterface;

namespace Ui {
    class SAKDebugger;
}

/// @brief Debugging page
class SAKDebugger : public QWidget
{
    Q_OBJECT
public:
    SAKDebugger(int type, QString name, QWidget *parent = Q_NULLPTR);
    ~SAKDebugger();

    /**
     * @brief outputMessage: Output message to ui
     * @param msg: The message that need to be show
     * @param isInfo: true-text color is green, false-text color is red
     */
    void outputMessage(QString msg, bool isInfo = true);

    /**
     * @brief pageType: The type of debugging page.(SAKDataStruct::SAKEnumDebugPageType)
     * @return Type of debugging page
     */
    quint32 pageType();

    /**
     * @brief settingsInstance: Get the settings instance
     * @return Settings instance
     */
    QSettings *settings();

    /**
     * @brief sqlDatabase: Get the database instance
     * @return Database instance
     */
    QSqlDatabase *sqlDatabase();

    /**
     * @brief settingsGroup: Get the settings group
     * @param pageType: The type of debug page
     * @return Settings group
     */
    QString settingsGroup();

    /**
     * @brief inputController: Get SAKDebugPageInputController instance pointer
     * @return SAKDebugPageInputController instance pointer
     */
    SAKDebuggerInput *inputController();

    /**
     * @brief outputController: Get SAKDebugPageOutputController instance pointer
     * @return SAKDebugPageOutputController instance pointer
     */
    SAKDebuggerOutput *outputController();

    /**
     * @brief statisticsController: Get SAKDebugPageStatisticsController instance
     * pointer
     * @return SAKDebugPageStatisticsController instance pointer
     */
    SAKDebuggerStatistics *statisticsController();

    /**
     * @brief deviceController: Get the device controller instance,
     * the controller will be destroy when the page is closed.
     * @return Device controller instance pointer
     */
    SAKDebuggerController *deviceController();

    /**
     * @brief databaseInterface: Get the data base read-write interface.
     * @return The interface.
     */
    SAKDebugPageCommonDatabaseInterface *databaseInterface();

    // Table name
    QString tableNameAutoResponseTable();
    QString tableNamePresettingDataTable();
    QString tableNameTimingSendingTable();
protected:
    /**
     * @brief createDevice: Get the device instance, the device will be destroy when
     * it is closed. You can not specify the parent of device(the return value).
     * @return Device instance pointer
     */
    virtual SAKDebuggerDevice* device() = 0;

    /**
     * @brief controller: Get the device controller of the debugger
     * @return Device controller
     */
    virtual SAKDebuggerController *controller() = 0;

    /**
     * @brief initDebugger: Initializing, the function must be called in the
     * constructor of subclass.
     */
    void initDebugger();
private:
    void initDebuggerController();
    void initDebuggerDevice();
    void initDebuggerStatistics();
    void initDebuggerOutout();
    void initDebuggerInput();
private:
    int mDebugPageType;
    QMenu *mDeviceMenu;
    QTimer mClearInfoTimer;
    QString mSettingGroup;
    QAction *mRefreshAction;
    SAKDebugPageCommonDatabaseInterface *mDatabaseInterface;

    // Debug page modules
    SAKDebuggerInput *mModuleInput;
    SAKDebuggerOutput *mModuleOutput;
    SAKDebuggerStatistics *mModuleStatistics;
    SAKDebuggerDevice *mModuleDevice;
    SAKDebuggerController *mModuleController;
    SAKDebuggerPlugins *mModulePlugins;
public:
    static void commonSqlApiUpdateRecord(QSqlQuery *sqlQuery,
                                         QString tableName,
                                         QString columnName,
                                         QVariant value,
                                         quint64 recordID,
                                         bool valueIsString);
    static void commonSqlApiDeleteRecord(QSqlQuery *sqlQuery,
                                         QString tableName,
                                         quint64 recordID);


private:
    Ui::SAKDebugger *mUi;
};

#endif  // SAKTABPAGE_HH
