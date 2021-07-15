/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
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
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebugPageController.hh"
#include "SAKOtherAnalyzerThread.hh"
#include "SAKOtherHighlighterManager.hh"
#include "SAKDebugPageOtherController.hh"
#include "SAKDebuggerOutput.hh"
#include "SAKOtherAnalyzerThreadManager.hh"
#include "SAKOtherAutoResponseItemManager.hh"
#include "SAKDebugPageStatisticsController.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#ifdef SAK_IMPORT_MODULE_CHARTS
#include "SAKDebugPageChartsController.hh"
#endif

#include "ui_SAKDebugger.h"

SAKDebugger::SAKDebugger(int type, QString name, QWidget *parent)
    :QWidget(parent)
    ,mIsInitializing(true)
    ,mDebugPageType(type)
    ,mSettingGroup(name)
    ,mChartsController(Q_NULLPTR)
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
    initializingVariables();

    mDatabaseInterface = new SAKDebugPageCommonDatabaseInterface(this, sakApp->sqlDatabase(), this);
    mOutputController = new SAKDebuggerOutput(mMoreOutputSettingsPushButton,
                                              mOutputTextFormatComboBox,
                                              settings(),
                                              settingsGroup(),
                                              mOutputTextBroswer,
                                              this);
    mOtherController = new SAKDebugPageOtherController(this, this);
    mStatisticsController = new SAKDebugPageStatisticsController(this, this);
    mInputController = new SAKDebuggerInput(this, this);
#ifdef SAK_IMPORT_MODULE_CHARTS
    mChartsController= new SAKDebugPageChartsController(this);
#endif

    // Initializing the timer.
    mClearInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearInfoTimer, &QTimer::timeout, this, &SAKDebugger::cleanInfo);
    mIsInitializing = false;
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
#ifdef SAK_IMPORT_MODULE_CHARTS
    delete mChartsController;
#endif
    delete mUi;
}

void SAKDebugger::write(QByteArray data)
{
    emit requestWriteData(data);
}

void SAKDebugger::writeRawData(QString rawData, int textFormat)
{
    emit requestWriteRawData(rawData, textFormat);
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
        mInfoLabel->setStyleSheet(QString("QLabel{color:black}"));
    }else{
        mInfoLabel->setStyleSheet(QString("QLabel{color:red}"));
        QApplication::beep();
    }

    msg.prepend(time);
    mInfoLabel->setText(msg);
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

SAKDebugPageOtherController *SAKDebugger::otherController()
{
    return mOtherController;
}

SAKDebuggerInput *SAKDebugger::inputController()
{
    return mInputController;
}

#ifdef SAK_IMPORT_MODULE_CHARTS
SAKDebugPageChartsController *SAKDebugger::chartsController()
{
   return mChartsController;
}
#endif

SAKDebuggerOutput *SAKDebugger::outputController()
{
    return mOutputController;
}

SAKDebugPageStatisticsController *SAKDebugger::statisticsController()
{
    return mStatisticsController;
}

SAKDebugPageController *SAKDebugger::deviceController()
{
    Q_ASSERT_X(mDeviceController, __FUNCTION__, "You must initialize mDeviceController in the subclass!");
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
        QHBoxLayout *layout = new QHBoxLayout(mControllerWidget);
        mControllerWidget->setLayout(layout);
        layout->addWidget(mDeviceController);
        layout->setContentsMargins(0, 0, 0, 0);
    }

    mDevice = device();
    Q_ASSERT_X(mDevice, __FUNCTION__, "You must initialize the mDevice in the subcalss!");

    connect(this, &SAKDebugger::requestWriteData, mDevice, &SAKDebuggerDevice::writeBytes);
    connect(mDevice, &SAKDebuggerDevice::bytesWritten, this, &SAKDebugger::bytesWritten);

    // The bytes read will be input to analyzer, after analyzing, the bytes will be input to debug page
    SAKOtherAnalyzerThreadManager *analyzerManager = mOtherController->analyzerThreadManager();
    connect(mDevice, &SAKDebuggerDevice::bytesRead, analyzerManager, &SAKOtherAnalyzerThreadManager::inputBytes);

    // The function may be called multiple times, so do something to ensure that the signal named bytesAnalysed
    // and the slot named bytesRead are connected once.
    connect(analyzerManager, &SAKOtherAnalyzerThreadManager::bytesAnalysed, this, &SAKDebugger::bytesRead, static_cast<Qt::ConnectionType>(Qt::AutoConnection|Qt::UniqueConnection));
    connect(mDevice, &SAKDebuggerDevice::messageChanged, this, &SAKDebugger::outputMessage);
    connect(mDevice, &SAKDebuggerDevice::deviceStateChanged, this, &SAKDebugger::changedDeviceState);
    connect(mDevice, &SAKDebuggerDevice::finished, this, &SAKDebugger::closeDevice, Qt::QueuedConnection);


    // Initialize the more button, the firs thing to do is clear the old actions and delete the old menu.
    auto deviceMorePushButtonMenu = mDeviceMorePushButton->menu();
    if (deviceMorePushButtonMenu){
        deviceMorePushButtonMenu->clear();
        deviceMorePushButtonMenu->deleteLater();
    }

    // Create "Refresh" action.
    deviceMorePushButtonMenu = new QMenu(mDeviceMorePushButton);
    mDeviceMorePushButton->setMenu(deviceMorePushButtonMenu);
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
    mCyclingTimeComboBox->setEnabled(opened);
    mSendPushButton->setEnabled(opened);
    mRefreshAction->setEnabled(!opened);
    mDeviceController->setUiEnable(opened);

    mSwitchPushButton->setEnabled(true);
}

void SAKDebugger::openDevice()
{
    mDevice->start();
    mSwitchPushButton->setText(tr("Close"));
}

void SAKDebugger::closeDevice()
{
    mDevice->requestInterruption();
    mDevice->requestWakeup();
    mDevice->exit();
    mDevice->wait();
    mSwitchPushButton->setText(tr("Open"));
}

void SAKDebugger::refreshDevice()
{
    if (mDeviceController){
        mDeviceController->refreshDevice();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "A null pointer!");
    }
}

void SAKDebugger::cleanInfo()
{
    mClearInfoTimer.stop();
    mInfoLabel->clear();
}

void SAKDebugger::on_switchPushButton_clicked()
{
    mSwitchPushButton->setEnabled(false);
    if (mDevice){
        if (!mDevice->isRunning()){
            openDevice();
        }else{
            closeDevice();
        }
    }
}

void SAKDebugger::initializingVariables()
{
    // Devce control
    mSwitchPushButton = mUi->switchPushButton;
    mDeviceMorePushButton = mUi->deviceMorePushButton;
    mControllerWidget = mUi->controllerWidget;

    // Message output
    mInfoLabel = mUi->infoLabel;

    // Input settings
    mCyclingTimeComboBox = mUi->cyclingTimeComboBox;
    mInputFormatComboBox = mUi->inputFormatComboBox;
    mMoreInputSettingsPushButton = mUi->moreInputSettingsPushButton;
    mSendPushButton = mUi->sendPushButton;
    mInputTextEdit = mUi->inputTextEdit;
    mCrcLabel = mUi->crcLabel;

    // Output settings
#if 0
    mRxLabel = mUi->rxLabel;
    mTxLabel = mUi->txLabel;
#endif
    mOutputTextFormatComboBox = mUi->outputTextFormatComboBox;
#if 0
    mAutoWrapCheckBox = mUi->autoWrapCheckBox;
    mShowDateCheckBox = mUi->showDateCheckBox;
    mShowTimeCheckBox = mUi->showTimeCheckBox;
    mShowMsCheckBox = mUi->showMsCheckBox;
    mShowRxDataCheckBox = mUi->showRxDataCheckBox;
    mShowTxDataCheckBox = mUi->showTxDataCheckBox;
    mSaveOutputToFileCheckBox = mUi->saveOutputToFileCheckBox;
    mRawDataCheckBox = mUi->rawDataCheckBox;
#endif
    mMoreOutputSettingsPushButton = mUi->moreOutputSettingsPushButton;
    mClearOutputPushButton = mUi->clearOutputPushButton;
    mOutputTextBroswer = mUi->outputTextBroswer;

    // Statistics
    mRxSpeedLabel = mUi->rxSpeedLabel;
    mTxSpeedLabel = mUi->txSpeedLabel;
    mRxFramesLabel = mUi->rxFramesLabel;
    mTxFramesLabel = mUi->txFramesLabel;
    mRxBytesLabel = mUi->rxBytesLabel;
    mTxBytesLabel = mUi->txBytesLabel;
#if 0
    mResetTxCountPushButton = mUi->resetTxCountPushButton;
    mResetRxCountPushButton = mUi->resetRxCountPushButton;
#endif
#if 0
    // Other settings
    mTransmissionSettingPushButton = mUi->transmissionSettingPushButton;
    mAutoResponseSettingPushButton = mUi->autoResponseSettingPushButton;
    mTimingSendingPushButton = mUi->timingSendingPushButton;
    mMoreSettingsPushButton = mUi->moreSettingsPushButton;
    mAnalyzerPushButton = mUi->analyzerPushButton;
#endif
}
