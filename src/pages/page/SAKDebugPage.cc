/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
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

#ifdef SAK_IMPORT_CHARTS_MODULE
#include "SAKDebugPageChartsController.hh"
#endif

#ifdef SAK_IMPORT_COM_MODULE
#include <QSerialPort>
#include <QSerialPortInfo>
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
    initializingVariables();

    mOutputController = new SAKDebugPageOutputController(this, this);
    mOtherController = new SAKDebugPageOtherController(this, this);
    mStatisticsController = new SAKDebugPageStatisticsController(this, this);
    mInputController = new SAKDebugPageInputController(this, this);
#ifdef SAK_IMPORT_CHARTS_MODULE
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
#ifdef SAK_IMPORT_CHARTS_MODULE
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

#ifdef SAK_IMPORT_CHARTS_MODULE
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
    return mDeviceController;
}

#ifdef SAK_IMPORT_COM_MODULE
void SAKDebugPage::initComComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<QSerialPortInfo> coms = QSerialPortInfo::availablePorts();
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        for(auto var:coms){
            QStandardItem *item = new QStandardItem(var.portName());
            item->setToolTip(var.description());
            itemModel->appendRow(item);
        }

        comboBox->setModel(itemModel);
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKDebugPage::initBaudRateComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QList<qint32> bd = QSerialPortInfo::standardBaudRates();
        for (auto var:bd) {
            comboBox->addItem(QString::number(var), QVariant::fromValue(var));
        }

        comboBox->setCurrentText("9600");
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKDebugPage::initDataBitsComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem("8", QVariant::fromValue(QSerialPort::Data8));
        comboBox->addItem("7", QVariant::fromValue(QSerialPort::Data7));
        comboBox->addItem("6", QVariant::fromValue(QSerialPort::Data6));
        comboBox->addItem("5", QVariant::fromValue(QSerialPort::Data5));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKDebugPage::initStopBitsComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem("1", QVariant::fromValue(QSerialPort::OneStop));
#ifdef Q_OS_WINDOWS
        comboBox->addItem("1.5", QVariant::fromValue(QSerialPort::OneAndHalfStop));
#endif
        comboBox->addItem("2", QVariant::fromValue(QSerialPort::TwoStop));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKDebugPage::initParityComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("No"), QVariant::fromValue(QSerialPort::NoParity));
        comboBox->addItem(tr("Even"), QVariant::fromValue(QSerialPort::EvenParity));
        comboBox->addItem(tr("Odd"), QVariant::fromValue(QSerialPort::OddParity));
        comboBox->addItem(tr("Space"), QVariant::fromValue(QSerialPort::SpaceParity));
        comboBox->addItem(tr("Mark"), QVariant::fromValue(QSerialPort::MarkParity));
    }
}
#endif

#ifdef SAK_IMPORT_COM_MODULE
void SAKDebugPage::initFlowControlComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(tr("No"), QVariant::fromValue(QSerialPort::NoFlowControl));
        comboBox->addItem(tr("Hardware"), QVariant::fromValue(QSerialPort::HardwareControl));
        comboBox->addItem(tr("Software"), QVariant::fromValue(QSerialPort::SoftwareControl));
    }
}
#endif

void SAKDebugPage::initIpComboBox(QComboBox *comboBox, bool appendHostAny)
{
    QString localHost("127.0.0.1");
    if (comboBox){
        comboBox->clear();
        comboBox->addItem(QString("::"));
        comboBox->addItem(QString("::1"));
        comboBox->addItem(QString("0.0.0.0"));
        comboBox->addItem(localHost);
        QList<QHostAddress> addresses = QNetworkInterface::allAddresses();
        for(auto var:addresses){
            if (var.protocol() == QAbstractSocket::IPv4Protocol) {
                if (var.toString().compare(localHost) == 0){
                    continue;
                }
                comboBox->addItem(var.toString());
            }
        }

        if (appendHostAny){
            comboBox->addItem(QString(SAK_HOST_ADDRESS_ANY));
        }
        comboBox->setCurrentText(localHost);
    }
}

void SAKDebugPage::initInputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKCommonDataStructure::InputFormatBin, tr("BIN"));
        formatMap.insert(SAKCommonDataStructure::InputFormatOct, tr("OTC"));
        formatMap.insert(SAKCommonDataStructure::InputFormatDec, tr("DEC"));
        formatMap.insert(SAKCommonDataStructure::InputFormatHex, tr("HEX"));
        formatMap.insert(SAKCommonDataStructure::InputFormatAscii, tr("ASCII"));
        formatMap.insert(SAKCommonDataStructure::InputFormatUtf8, tr("UTF8"));
        formatMap.insert(SAKCommonDataStructure::InputFormatLocal, tr("SYSTEM"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKCommonDataStructure::InputFormatLocal));

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }
    }
}

void SAKDebugPage::initOutputTextFormatComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();

        QMap<int, QString> formatMap;
        formatMap.insert(SAKCommonDataStructure::OutputFormatBin, tr("BIN"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatDec, tr("DEC"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatHex, tr("HEX"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatAscii, tr("ASCII"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUtf8, tr("UTF8"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUtf16, tr("UTF16"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatUcs4, tr("UCS4"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatStdwstring, tr("WChart"));
        formatMap.insert(SAKCommonDataStructure::OutputFormatLocal, tr("SYSTEM"));

        QMapIterator<int, QString> mapIterator(formatMap);
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        while (mapIterator.hasNext()) {
            mapIterator.next();
            QStandardItem *item = new QStandardItem(mapIterator.value());
            item->setToolTip(mapIterator.value());
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);
        comboBox->setCurrentText(formatMap.value(SAKCommonDataStructure::OutputFormatHex));

        // Reset the iterator...
        while (mapIterator.hasPrevious()) {
            mapIterator.previous();
        }

        // Set item data of combo box
        int index = 0;
        while (mapIterator.hasNext()) {
            mapIterator.next();
            comboBox->setItemData(index, QVariant::fromValue(mapIterator.key()));
            index += 1;
        }
    }
}

void SAKDebugPage::initCRCComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->clear();
        QMetaEnum enums = QMetaEnum::fromType<SAKCommonCrcInterface::CRCModel>();
        QStandardItemModel *itemModel = new QStandardItemModel(comboBox);
        for (int i = 0; i < enums.keyCount(); i++){
            const QString key = enums.key(i);
            // There may be a bug, I do not know whether will the itemModel take ownership of the item
            // if not, a memory leak will occur after comboBox is destroyed.
            QStandardItem *item = new QStandardItem(key);
            item->setToolTip(key);
            itemModel->appendRow(item);
        }
        comboBox->setModel(itemModel);

        // add item data
        for (int i = 0; i < comboBox->count(); i++){
            for (int j = 0; j < enums.keyCount(); j++){
                if (comboBox->itemText(i) == QString(enums.key(j))){
                    comboBox->setItemData(i, enums.value(j));
                    break;
                }
            }
        }
    }
}

void SAKDebugPage::initWebSocketSendingTypeComboBox(QComboBox *comboBox)
{
    if (comboBox){
        comboBox->addItem(tr("BIN"), SAKCommonDataStructure::WebSocketSendingTypeBin);
        comboBox->addItem(tr("TEXT"), SAKCommonDataStructure::WebSocketSendingTypeText);
    }
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
    // Create new menu and add actions to the menu.
    auto infos = device()->settingsPanelList();
    deviceMorePushButtonMenu = new QMenu;
    mDeviceMorePushButton->setMenu(deviceMorePushButtonMenu);
    for (auto var : infos){
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
    mSendPushButton->setEnabled(opened);
    mSendPresetPushButton->setEnabled(opened);
    mCycleEnableCheckBox->setEnabled(opened);
    mRefreshPushButton->setEnabled(!opened);
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

void SAKDebugPage::cleanInfo()
{
    mClearInfoTimer.stop();
    mInfoLabel->clear();
}

void SAKDebugPage::setupController()
{

}

void SAKDebugPage::on_refreshPushButton_clicked()
{
    if (mDeviceController){
        mDeviceController->refreshDevice();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "A null pointer!");
    }
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
    mRefreshPushButton = mUi->refreshPushButton;
    mDeviceMorePushButton = mUi->deviceMorePushButton;
    mDeviceSettingFrame = mUi->deviceSettingFrame;

    // Message output
    mInfoLabel = mUi->infoLabel;

    // Input settings
    mInputModelComboBox = mUi->inputModelComboBox;
    mCycleEnableCheckBox = mUi->cycleEnableCheckBox;
    mCycleTimeLineEdit = mUi->cycleTimeLineEdit;
    mSaveInputDataPushButton = mUi->saveInputDataPushButton;
    mReadinFilePushButton = mUi->readinFilePushButton;
    mAddCRCCheckBox = mUi->addCRCCheckBox;
    mCrcSettingsPushButton = mUi->crcSettingsPushButton;
    mClearInputPushButton = mUi->clearInputPushButton;
    mSendPushButton = mUi->sendPushButton;
    mInputTextEdit = mUi->inputTextEdit;
    mCrcParameterModelsComboBox = mUi->crcParameterModelsComboBox;
    mCrcLabel = mUi->crcLabel;
    mPresetPushButton = mUi->presetPushButton;
    mSendPresetPushButton = mUi->sendPresetPushButton;

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
    mOutputFilePathPushButton = mUi->outputFilePathPushButton;
    mClearOutputPushButton = mUi->clearOutputPushButton;
    mSaveOutputPushButton = mUi->saveOutputPushButton;
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
    mAnalyzerPushButton = mUi->analyzerPushButton;
    mAutoResponseSettingPushButton = mUi->autoResponseSettingPushButton;
    mTimingSendingPushButton = mUi->timingSendingPushButton;
    mHighlightSettingPushButton = mUi->highlightSettingPushButton;
    mMoreSettingsPushButton = mUi->moreSettingsPushButton;

    // Charts
    mDataVisualizationPushButton = mUi->dataVisualizationPushButton;
}

void SAKDebugPage::on_dataVisualizationPushButton_clicked()
{
#ifdef SAK_IMPORT_CHARTS_MODULE
    if (mChartsController->isHidden()){
        mChartsController->show();
    }else{
        mChartsController->activateWindow();
    }
#else
    QMessageBox::warning(this, tr("Unsupported function"), tr("The function has been disable, beause of developer's Qt version is not supported!"));
#endif
}
