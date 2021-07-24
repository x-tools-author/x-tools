/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMenu>
#include <QAction>
#include <QPixmap>
#include <QDateTime>
#include <QSettings>
#include <QKeyEvent>
#include <QMetaEnum>
#include <QScrollBar>
#include <QMessageBox>
#include <QHBoxLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QHostAddress>
#include <QIntValidator>
#include <QLoggingCategory>
#include <QNetworkInterface>
#include <QStandardItemModel>

#include "SAKDebugger.hh"
#include "SAKApplication.hh"
#include "SAKDebuggerInput.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerOutput.hh"
#include "SAKDebuggerPlugins.hh"
#include "SAKDebuggerStatistics.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebugPageController.hh"
#include "SAKOtherAnalyzerThread.hh"
#include "SAKOtherAnalyzerThreadManager.hh"
#include "SAKPluginAutomaticallyResponse.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#ifdef SAK_IMPORT_MODULE_CHARTS
#include "SAKPluginCharts.hh"
#endif

#include "ui_SAKDebugger.h"

SAKDebugger::SAKDebugger(int type, QString name, QWidget *parent)
    :QWidget(parent)
    ,mIsInitializing(true)
    ,mDebugPageType(type)
    ,mSettingGroup(name)
    ,mDevice(Q_NULLPTR)
    ,mDeviceController(Q_NULLPTR)
    ,mUi(new Ui::SAKDebugger)
{
    mUi->setupUi(this);
    auto html = mUi->outputTextBroswer->toHtml();
    html = html.replace(QString("1970"), sakApp->buildDate()->toString("yyyy"));
    html = html.replace(QString("Author"), QString(SAK_AUTHOR));
    html = html.replace(QString("Email"), QString(SAK_AUTHOR_EMAIL));
    mUi->outputTextBroswer->setHtml(html);

    mDatabaseInterface = new SAKDebugPageCommonDatabaseInterface(this,
                                                                 sakApp->sqlDatabase(),
                                                                 this);
    mPlugins = new SAKDebuggerPlugins(mUi->readmePushButton,
                                      mUi->pluginsPushButton,
                                      settings(),
                                      settingsGroup(),
                                      mUi->pluginPanelLabel,
                                      mUi->pluginPanelWidget,
                                      this);
    mOutputController = new SAKDebuggerOutput(mUi->moreOutputSettingsPushButton,
                                              mUi->outputTextFormatComboBox,
                                              settings(),
                                              settingsGroup(),
                                              mUi->outputTextBroswer,
                                              this);
    mStatistics = new SAKDebuggerStatistics(mUi->txSpeedLabel,
                                            mUi->rxSpeedLabel,
                                            mUi->txFramesLabel,
                                            mUi->rxFramesLabel,
                                            mUi->txBytesLabel,
                                            mUi->rxBytesLabel,
                                            this);
    mInputController = new SAKDebuggerInput(mUi->cyclingTimeComboBox,
                                            mUi->inputFormatComboBox,
                                            mUi->moreInputSettingsPushButton,
                                            mUi->sendPushButton,
                                            mUi->crcLabel,
                                            mUi->inputComboBox,
                                            settings(),
                                            mSettingGroup,
                                            sqlDatabase(),
                                            this);
    mInputController->updateUiState(false);

    // Initializing the timer.
    mClearInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearInfoTimer, &QTimer::timeout, this, &SAKDebugger::cleanInfo);
    mIsInitializing = false;

    connect(mUi->switchPushButton, &QPushButton::clicked,
            this, [=](){
        mUi->switchPushButton->setEnabled(false);
        if (this->device()){
            if (!this->device()->isRunning()){
                openDevice();
            }else{
                closeDevice();
            }
        }
    });
}

SAKDebugger::~SAKDebugger()
{
#if 1
    if (mDevice) {
        if (mDevice->isRunning()){
            mDevice->requestInterruption();
            mDevice->wait();
        }
        mDevice->deleteLater();
    }
#endif
    delete mUi;
}

void SAKDebugger::outputMessage(QString msg, bool isInfo)
{
    //mOutputController->outputLog(msg, isInfo);
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss ");
    QString temp;
    temp.append(time);
    temp.append(msg);
    time = QString("<font color=silver>%1</font>").arg(time);

    if (isInfo){
        mUi->infoLabel->setStyleSheet(QString("QLabel{color:black}"));
    }else{
        mUi->infoLabel->setStyleSheet(QString("QLabel{color:red}"));
        QApplication::beep();
    }

    msg.prepend(time);
    mUi->infoLabel->setText(msg);
    mClearInfoTimer.start();
}

quint32 SAKDebugger::pageType()
{
    return mDebugPageType;
}

QSettings *SAKDebugger::settings()
{
    return static_cast<SAKApplication*>(qApp)->settings();
}

QSqlDatabase *SAKDebugger::sqlDatabase()
{
    return qobject_cast<SAKApplication*>(qApp)->sqlDatabase();
}

QString SAKDebugger::settingsGroup()
{
    return mSettingGroup;
}

SAKDebuggerInput *SAKDebugger::inputController()
{
    return mInputController;
}

SAKDebuggerOutput *SAKDebugger::outputController()
{
    return mOutputController;
}

SAKDebuggerStatistics *SAKDebugger::statisticsController()
{
    return mStatistics;
}

SAKDebugPageController *SAKDebugger::deviceController()
{
    Q_ASSERT_X(mDeviceController,
               __FUNCTION__,
               "You must initialize mDeviceController in the subclass!");
    return mDeviceController;
}

SAKDebugPageCommonDatabaseInterface *SAKDebugger::databaseInterface()
{
    return mDatabaseInterface;
}

QString SAKDebugger::tableNameAutoResponseTable()
{
    return settingsGroup().append(QString("AutoResponse"));
}

QString SAKDebugger::tableNamePresettingDataTable()
{
    return settingsGroup().append(QString("PresettingData"));
}

QString SAKDebugger::tableNameTimingSendingTable()
{
    return settingsGroup().append(QString("TimingSending"));
}

void SAKDebugger::initializePage()
{
    mDeviceController = controller();
    if (mDeviceController){
        QHBoxLayout *layout = new QHBoxLayout(mUi->controllerWidget);
        mUi->controllerWidget->setLayout(layout);
        layout->addWidget(mDeviceController);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    mDevice = device();
    Q_ASSERT_X(mDevice,
               __FUNCTION__,
               "You must initialize the mDevice in the subcalss!");


    // Statistics
    connect(mDevice, &SAKDebuggerDevice::bytesRead,
            mStatistics, &SAKDebuggerStatistics::onBytesRead);
    connect(mDevice, &SAKDebuggerDevice::bytesWritten,
            mStatistics, &SAKDebuggerStatistics::onBytesWritten);


    // Outout
    connect(mDevice, &SAKDebuggerDevice::bytesRead,
            mOutputController, &SAKDebuggerOutput::onBytesRead);
    connect(mDevice, &SAKDebuggerDevice::bytesWritten,
            mOutputController, &SAKDebuggerOutput::onBytesWritten);


    // Input
    connect(mInputController, &SAKDebuggerInput::invokeWriteBytes,
            mDevice, &SAKDebuggerDevice::writeBytes, Qt::QueuedConnection);


    // The function may be called multiple times,
    // so do something to ensure that the signal named bytesAnalysed
    // and the slot named bytesRead are connected once.
    connect(mDevice, &SAKDebuggerDevice::messageChanged,
            this, &SAKDebugger::outputMessage);
    connect(mDevice, &SAKDebuggerDevice::deviceStateChanged,
            this, &SAKDebugger::changedDeviceState);
    connect(mDevice, &SAKDebuggerDevice::finished,
            this, &SAKDebugger::closeDevice, Qt::QueuedConnection);


    // Initialize the more button,
    // the first thing to do is clear the old actions and delete the old menu.
    auto deviceMorePushButtonMenu = mUi->deviceMorePushButton->menu();
    if (deviceMorePushButtonMenu){
        deviceMorePushButtonMenu->clear();
        deviceMorePushButtonMenu->deleteLater();
    }

    // Create "Refresh" action.
    deviceMorePushButtonMenu = new QMenu(mUi->deviceMorePushButton);
    mUi->deviceMorePushButton->setMenu(deviceMorePushButtonMenu);
    mRefreshAction = new QAction(tr("Refresh"), deviceMorePushButtonMenu);
    deviceMorePushButtonMenu->addAction(mRefreshAction);
    connect(mRefreshAction, &QAction::triggered, this, &SAKDebugger::refreshDevice);

    // Create new menu and add actions to the menu.
    auto infos = mDevice->settingsPanelList();
    for (auto &var : infos){
        QAction *action = new QAction(var.name, deviceMorePushButtonMenu);
        deviceMorePushButtonMenu->addAction(action);
        connect(action, &QAction::triggered, [=](){
            if (var.panel->isHidden()){
                var.panel->show();
            }else{
                var.panel->show();
            }
        });
    }
}

void SAKDebugger::changedDeviceState(bool opened)
{
    mInputController->updateUiState(opened);
    mRefreshAction->setEnabled(!opened);
    mDeviceController->setUiEnable(opened);

    mUi->switchPushButton->setEnabled(true);
}

void SAKDebugger::openDevice()
{
    mDevice->start();
    mUi->switchPushButton->setText(tr("Close"));
}

void SAKDebugger::closeDevice()
{
    mDevice->requestInterruption();
    mDevice->requestWakeup();
    mDevice->exit();
    mDevice->wait();
    mUi->switchPushButton->setText(tr("Open"));
}

void SAKDebugger::refreshDevice()
{
    if (mDeviceController){
        mDeviceController->refreshDevice();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "A null pointer!");
    }
}

void SAKDebugger::commonSqlApiUpdateRecord(QSqlQuery *sqlQuery,
                                           QString tableName,
                                           QString columnName,
                                           QVariant value,
                                           quint64 recordID,
                                           bool valueIsString)
{
    QString queryString;
    if (valueIsString){
        queryString = QString("UPDATE %1 SET %2='%3' WHERE ID=%4")
                .arg(tableName,
                     columnName,
                     value.toString(),
                     QString::number(recordID));
    }else{
        queryString = QString("UPDATE %1 SET %2=%3 WHERE ID=%4")
                .arg(tableName,
                     columnName,
                     value.toString(),
                     QString::number(recordID));
    }

    if(!sqlQuery->exec(queryString)){
        qWarning() << QString("Can not update record(%1):%2")
                      .arg(columnName, sqlQuery->lastError().text());
    }
}

void SAKDebugger::commonSqlApiDeleteRecord(QSqlQuery *sqlQuery,
                                           QString tableName,
                                           quint64 recordID)
{
    QString queryString = QString("DELETE FROM %1 WHERE ID=%2")
            .arg(tableName)
            .arg(recordID);
    if (!sqlQuery->exec(queryString)) {
        qWarning() << "Can not delete recored form(" << tableName << ")"
                   << sqlQuery->lastError().text();
    }
}

void SAKDebugger::cleanInfo()
{
    mClearInfoTimer.stop();
    mUi->infoLabel->clear();
}
