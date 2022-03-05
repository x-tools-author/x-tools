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
class SAKDebuggerPluginTransponders;

namespace Ui {
    class SAKDebugger;
}

/// @brief Debugging page
class SAKDebugger : public QWidget
{
    Q_OBJECT
public:
    SAKDebugger(QSettings *settings,
                const QString settingsGroup,
                QSqlDatabase *sqlDatabase,
                QWidget *parent = Q_NULLPTR);
    ~SAKDebugger();
    void setStatisticsWidgetHiden(bool hiden);
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
     * @brief addMenu: Add action to menu.
     * @param menu: The device menu.
     */
    virtual void addActionToDeviceMenu(QMenu *menu){Q_UNUSED(menu);}
    virtual void updateUiState(bool opened){Q_UNUSED(opened)}

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
    void initDebuggerPlugin();
private:
    QMenu *mDeviceMenu;
    QTimer mClearInfoTimer;
    QAction *mRefreshAction;

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
    static int clearDataMessageBox();


private:
    Ui::SAKDebugger *mUi;
};

#endif  // SAKTABPAGE_HH
