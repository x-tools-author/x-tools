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

#include "SAKDebugPage.hh"
#include "SAKApplication.hh"
#include "SAKDebugPageDevice.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebugPageController.hh"
#include "SAKOtherAnalyzerThread.hh"
#include "SAKOtherHighlighterManager.hh"
#include "SAKDebugPageOtherController.hh"
#include "SAKDebugPageInputController.hh"
#include "SAKDebugPageOutputController.hh"
#include "SAKOtherAnalyzerThreadManager.hh"
#include "SAKOtherAutoResponseItemManager.hh"
#include "SAKDebugPageStatisticsController.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#ifdef SAK_IMPORT_MODULE_CHARTS
#include "SAKDebugPageChartsController.hh"
#endif

#include "ui_SAKDebugPage.h"

SAKDebugPage::SAKDebugPage(int type, QString name, QWidget *parent)
    :QWidget(parent)
    ,mDevice(Q_NULLPTR)
    ,mDeviceController(Q_NULLPTR)
    ,mIsInitializing(true)
    ,mDebugPageType(type)
    ,mSettingGroup(name)
    ,mUi(new Ui::SAKDebugPage)
{
    mUi->setupUi(this);
    auto html = mUi->outputTextBroswer->toHtml();
    html = html.replace(QString("1970"), sakApp->buildDate()->toString("yyyy"));
    html = html.replace(QString("Author"), QString(SAK_AUTHOR));
    html = html.replace(QString("Email"), QString(SAK_AUTHOR_EMAIL));
    mUi->outputTextBroswer->setHtml(html);
    initializingVariables();

    mDatabaseInterface = new SAKDebugPageCommonDatabaseInterface(this, sakApp->sqlDatabase(), this);
    mOutputController = new SAKDebugPageOutputController(this, this);
    mOtherController = new SAKDebugPageOtherController(this, this);
    mStatisticsController = new SAKDebugPageStatisticsController(this, this);
    mInputController = new SAKDebugPageInputController(this, this);
#ifdef SAK_IMPORT_MODULE_CHARTS
    mChartsController= new SAKDebugPageChartsController(this);
#endif

    // Initializing the timer.
    mClearInfoTimer.setInterval(SAK_CLEAR_MESSAGE_INTERVAL);
    connect(&mClearInfoTimer, &QTimer::timeout, this, &SAKDebugPage::cleanInfo);
    mIsInitializing = false;
}

SAKDebugPage::~SAKDebugPage()
{
    if (device()->isRunning()){
        device()->requestInterruption();
        device()->wait();
    }
    delete device();
#ifdef SAK_IMPORT_MODULE_CHARTS
    delete mChartsController;
#endif
    delete mUi;
}

void SAKDebugPage::write(QByteArray data)
{
    emit requestWriteData(data);
}

void SAKDebugPage::writeRawData(QString rawData, int textFormat)
{
    emit requestWriteRawData(rawData, textFormat);
}

void SAKDebugPage::outputMessage(QString msg, bool isInfo)
{
    mOutputController->outputLog(msg, isInfo);
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

quint32 SAKDebugPage::pageType()
{
    return mDebugPageType;
}

QSettings *SAKDebugPage::settings()
{
    return static_cast<SAKApplication*>(qApp)->settings();
}

QSqlDatabase *SAKDebugPage::sqlDatabase()
{
    return qobject_cast<SAKApplication*>(qApp)->sqlDatabase();
}

QString SAKDebugPage::settingsGroup()
{
    return mSettingGroup;
}

SAKDebugPageOtherController *SAKDebugPage::otherController()
{
    return mOtherController;
}

SAKDebugPageInputController *SAKDebugPage::inputController()
{
    return mInputController;
}

#ifdef SAK_IMPORT_MODULE_CHARTS
SAKDebugPageChartsController *SAKDebugPage::chartsController()
{
   return mChartsController;
}
#endif

SAKDebugPageOutputController *SAKDebugPage::outputController()
{
    return mOutputController;
}

SAKDebugPageStatisticsController *SAKDebugPage::statisticsController()
{
    return mStatisticsController;
}

SAKDebugPageController *SAKDebugPage::deviceController()
{
    Q_ASSERT_X(mDeviceController, __FUNCTION__, "You must initialize mDeviceController in the subclass!");
    return mDeviceController;
}

SAKDebugPageCommonDatabaseInterface *SAKDebugPage::databaseInterface()
{
    return mDatabaseInterface;
}

QString SAKDebugPage::tableNameAutoResponseTable()
{
    return settingsGroup().append(QString("AutoResponse"));
}

QString SAKDebugPage::tableNamePresettingDataTable()
{
    return settingsGroup().append(QString("PresettingData"));
}

QString SAKDebugPage::tableNameTimingSendingTable()
{
    return settingsGroup().append(QString("TimingSending"));
}

SAKDebugPageDevice *SAKDebugPage::device()
{
    Q_ASSERT_X(mDevice, __FUNCTION__, "You must initialize the mDevice in the subcalss!");
    return mDevice;
}

void SAKDebugPage::initializePage()
{
    SAKDebugPageController *controller = deviceController();
    if (controller){
        QHBoxLayout *layout = new QHBoxLayout(mDeviceSettingFrame);
        mDeviceSettingFrame->setLayout(layout);
        layout->addWidget(controller);
        layout->setContentsMargins(0, 0, 0, 0);
        mDeviceController = controller;
    }

    connect(this, &SAKDebugPage::requestWriteData, device(), &SAKDebugPageDevice::writeBytes);
    connect(device(), &SAKDebugPageDevice::bytesWritten, this, &SAKDebugPage::bytesWritten);
#if 0
    connect(device, &SAKDevice::bytesRead, this, &SAKDebugPage::bytesRead);
#else
    // The bytes read will be input to analyzer, after analyzing, the bytes will be input to debug page
    SAKOtherAnalyzerThreadManager *analyzerManager = mOtherController->analyzerThreadManager();
    connect(device(), &SAKDebugPageDevice::bytesRead, analyzerManager, &SAKOtherAnalyzerThreadManager::inputBytes);

    // The function may be called multiple times, so do something to ensure that the signal named bytesAnalysed
    // and the slot named bytesRead are connected once.
    connect(analyzerManager, &SAKOtherAnalyzerThreadManager::bytesAnalysed, this, &SAKDebugPage::bytesRead, static_cast<Qt::ConnectionType>(Qt::AutoConnection|Qt::UniqueConnection));
#endif
    connect(device(), &SAKDebugPageDevice::messageChanged, this, &SAKDebugPage::outputMessage);
    connect(device(), &SAKDebugPageDevice::deviceStateChanged, this, &SAKDebugPage::changedDeviceState);
    connect(device(), &SAKDebugPageDevice::finished, this, &SAKDebugPage::closeDevice, Qt::QueuedConnection);


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
    connect(mRefreshAction, &QAction::triggered, this, &SAKDebugPage::refreshDevice);

    // Create new menu and add actions to the menu.
    auto infos = device()->settingsPanelList();
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

void SAKDebugPage::changedDeviceState(bool opened)
{
    mCyclingTimeComboBox->setEnabled(opened);
    mSendPushButton->setEnabled(opened);
    mRefreshAction->setEnabled(!opened);
    mDeviceController->setUiEnable(opened);

    mSwitchPushButton->setEnabled(true);
}

void SAKDebugPage::openDevice()
{
    device()->start();
    mSwitchPushButton->setText(tr("Close"));
}

void SAKDebugPage::closeDevice()
{
    device()->requestInterruption();
    device()->requestWakeup();
    device()->exit();
    device()->wait();
    mSwitchPushButton->setText(tr("Open"));
}

void SAKDebugPage::refreshDevice()
{
    if (mDeviceController){
        mDeviceController->refreshDevice();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "A null pointer!");
    }
}

void SAKDebugPage::cleanInfo()
{
    mClearInfoTimer.stop();
    mInfoLabel->clear();
}

void SAKDebugPage::on_switchPushButton_clicked()
{
    mSwitchPushButton->setEnabled(false);
    if (device()){
        if (!device()->isRunning()){
            openDevice();
        }else{
            closeDevice();
        }
    }
}

void SAKDebugPage::initializingVariables()
{
    // Devce control
    mSwitchPushButton = mUi->switchPushButton;
    mDeviceMorePushButton = mUi->deviceMorePushButton;
    mDeviceSettingFrame = mUi->deviceSettingFrame;

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
    mRxLabel = mUi->rxLabel;
    mTxLabel = mUi->txLabel;
    mOutputTextFormatComboBox= mUi->outputTextFormatComboBox;
    mAutoWrapCheckBox = mUi->autoWrapCheckBox;
    mShowDateCheckBox = mUi->showDateCheckBox;
    mShowTimeCheckBox = mUi->showTimeCheckBox;
    mShowMsCheckBox = mUi->showMsCheckBox;
    mShowRxDataCheckBox = mUi->showRxDataCheckBox;
    mShowTxDataCheckBox = mUi->showTxDataCheckBox;
    mSaveOutputToFileCheckBox = mUi->saveOutputToFileCheckBox;
    mRawDataCheckBox = mUi->rawDataCheckBox;
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
    mResetTxCountPushButton = mUi->resetTxCountPushButton;
    mResetRxCountPushButton = mUi->resetRxCountPushButton;

    // Other settings
    mTransmissionSettingPushButton = mUi->transmissionSettingPushButton;
    mAutoResponseSettingPushButton = mUi->autoResponseSettingPushButton;
    mTimingSendingPushButton = mUi->timingSendingPushButton;
    mMoreSettingsPushButton = mUi->moreSettingsPushButton;
    mAnalyzerPushButton = mUi->analyzerPushButton;

    // Charts
    mDataVisualizationPushButton = mUi->dataVisualizationPushButton;
}

void SAKDebugPage::on_dataVisualizationPushButton_clicked()
{
#ifdef SAK_IMPORT_MODULE_CHARTS
    if (mChartsController->isHidden()){
        mChartsController->show();
    }else{
        mChartsController->activateWindow();
    }
#else
    QMessageBox::warning(this, tr("Unsupported function"), tr("The function has been disable, beause of developer's Qt version is not supported!"));
#endif
}
