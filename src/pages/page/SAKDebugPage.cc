/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
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
#include <QIntValidator>
#include <QLoggingCategory>

#include "SAKDebugPageDevice.hh"
#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKProtocolAnalyzer.hh"
#include "SAKDebugPageStatisticsController.hh"
#include "SAKMoreSettingsWidget.hh"
#include "SAKDebugPageOtherController.hh"
#include "SAKDebugPageInputController.hh"
#include "SAKDebugPageOutputController.hh"
#include "SAKProtocolAnalyzerWidget.hh"
#include "SAKHighlightSettingsWidget.hh"
#ifdef SAK_IMPORT_CHARTS_MODULE
#include "SAKChartsController.hh"
#endif
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKDebugPage.h"

#define MINI_READ_WRITE_WATINGT_TIME 5   // 读写等待最小时间(单位为：ms)

SAKDebugPage::SAKDebugPage(int type, QWidget *parent)
    :QWidget(parent)
    ,mDevice(Q_NULLPTR)    
    ,mDebugPageType(type)
    ,mUi(new Ui::SAKDebugPage)
{
    mDatabaseInterface = SAKDebugPageDatabaseInterface::instance();

    mIsInitializing = true;
    initSettingString();

    mUi->setupUi(this);
    initUiPointer();

    mOutputManager           = new SAKDebugPageOutputController(this, this);
    mOtherSettings           = new SAKDebugPageOtherController(this, this);
    mStatisticsManager       = new SAKDebugPageStatisticsController(this, this);
    mDebugPageInputManager   = new SAKDebugPageInputController(this, this);
#ifdef SAK_IMPORT_CHARTS_MODULE
    mDataVisualizationManager= Q_NULLPTR;
#endif

    mRreadWriteParameters.waitForReadyReadTime = MINI_READ_WRITE_WATINGT_TIME;
    mRreadWriteParameters.waitForBytesWrittenTime = MINI_READ_WRITE_WATINGT_TIME;
    mRreadWriteParameters.runIntervalTime = 25;
#if 0
    resize(800, 600);
#endif
    mClearInfoTimer.setInterval(8*1000);
    connect(&mClearInfoTimer, &QTimer::timeout, this, &SAKDebugPage::cleanInfo);

    readinSettings();
    mIsInitializing = false;
}

SAKDebugPage::~SAKDebugPage()
{
    if (mDevice){
        mDevice->requestInterruption();
        mDevice->wakeMe();
        mDevice->exit();
        mDevice->wait();
        delete mDevice;
        mDevice = Q_NULLPTR;
    }

    if (mUi){
        delete mUi;
        mUi = Q_NULLPTR;
    }

#ifdef SAK_IMPORT_CHARTS_MODULE
    if (mDataVisualizationManager){
        delete mDataVisualizationManager;
        mDataVisualizationManager = Q_NULLPTR;
    }
#endif
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

struct SAKDebugPage::ReadWriteParameters SAKDebugPage::readWriteParameters()
{
    ReadWriteParameters parameters;
    mReadWriteParametersMutex.lock();
    parameters.waitForReadyReadTime = mRreadWriteParameters.waitForReadyReadTime;
    parameters.waitForBytesWrittenTime = mRreadWriteParameters.waitForBytesWrittenTime;
    parameters.runIntervalTime = mRreadWriteParameters.runIntervalTime;
    mReadWriteParametersMutex.unlock();

    return  parameters;
}

void SAKDebugPage::setReadWriteParameters(struct ReadWriteParameters parameters)
{
    if (parameters.waitForReadyReadTime < MINI_READ_WRITE_WATINGT_TIME){
        parameters.waitForReadyReadTime = MINI_READ_WRITE_WATINGT_TIME;
    }

    if (parameters.waitForBytesWrittenTime < MINI_READ_WRITE_WATINGT_TIME){
        parameters.waitForBytesWrittenTime = MINI_READ_WRITE_WATINGT_TIME;
    }

    mReadWriteParametersMutex.lock();
    mRreadWriteParameters.waitForReadyReadTime = parameters.waitForReadyReadTime;
    mRreadWriteParameters.waitForBytesWrittenTime = parameters.waitForBytesWrittenTime;
    mReadWriteParametersMutex.unlock();
}

SAKDebugPageDatabaseInterface *SAKDebugPage::databaseInterfaceInstance()
{
   return mDatabaseInterface;
}

quint32 SAKDebugPage::pageType()
{
    return mDebugPageType;
}

void SAKDebugPage::refreshDevice()
{

}

QWidget *SAKDebugPage::controllerWidget()
{
    return Q_NULLPTR;
}

SAKDebugPageDevice *SAKDebugPage::createDevice()
{
    return Q_NULLPTR;
}

void SAKDebugPage::setUiEnable(bool ebable)
{
    Q_UNUSED(ebable);
}

void SAKDebugPage::initPage()
{
    setupController();
    setupDevice();
}

void SAKDebugPage::changedDeviceState(bool opened)
{
    mSendPushButton->setEnabled(opened);
    mSendPresetPushButton->setEnabled(opened);
    mCycleEnableCheckBox->setEnabled(opened);

    setUiEnable(!opened);
}

void SAKDebugPage::initSettingKey()
{
    switch (mDebugPageType) {
#ifdef SAK_IMPORT_COM_MODULE
    case SAKDataStruct::DebugPageTypeCOM:
        mSettingKey = QString("COM");
        break;
#endif
#ifdef SAK_IMPORT_HID_MODULE
        case SAKDataStruct::DebugPageTypeHID:
        settingKey = QString("HID");
        break;
#endif
#ifdef SAK_IMPORT_USB_MODULE
    case SAKDataStruct::DebugPageTypeUSB:
    settingKey = QString("USB");
    break;
#endif
    case SAKDataStruct::DebugPageTypeUDP:
        mSettingKey = QString("UDP");
        break;
    case SAKDataStruct::DebugPageTypeTCPClient:
        mSettingKey = QString("TCPC");
        break;
    case SAKDataStruct::DebugPageTypeTCPServer:
        mSettingKey = QString("TCPS");
        break;
    case SAKDataStruct::DebugPageTypeWebSocketClient:
        mSettingKey = QString("WEBSOCKETC");
        break;
    case SAKDataStruct::DebugPageTypeWebSocketServer:
        mSettingKey = QString("WEBSOCKETS");
        break;
    default:
        mSettingKey = QString("UnknowDebugPage");
        Q_ASSERT_X(false, __FUNCTION__, "Invalid type of debug page");
        break;
    }
}

void SAKDebugPage::initSettingString()
{
    initSettingKey();
    initInputSettingString();
    initOutputSettingString();
}

void SAKDebugPage::initInputSettingString()
{
    mSettingStringInputModel        = QString("%1/inputModel").arg(mSettingKey);
    mSettingStringCycleTime         = QString("%1/cycleTime").arg(mSettingKey);
    mSettingStringAddCRC            = QString("%1/addCRC").arg(mSettingKey);
    mSettingStringBigeEndian        = QString("%1/bigeEndian").arg(mSettingKey);
    mSettingStringcrcParameterModel = QString("%1/parameterModel").arg(mSettingKey);
}

void SAKDebugPage::initOutputSettingString()
{
    mSettingStringOutputTextFormat = QString("%1/outputTextFormat").arg(mSettingKey);
    mSettingStringShowDate    = QString("%1/showDate").arg(mSettingKey);
    mSettingStringAutoWrap    = QString("%1/autoWrap").arg(mSettingKey);
    mSettingStringShowTime    = QString("%1/showTime").arg(mSettingKey);
    mSettingStringShowMs      = QString("%1/showMs").arg(mSettingKey);
    mSettingStringShowRx      = QString("%1/showRx").arg(mSettingKey);
    mSettingStringShowTx      = QString("%1/showTx").arg(mSettingKey);
}

void SAKDebugPage::readinSettings()
{
    readinInputSettings();
    readinOutputSettings();
}

void SAKDebugPage::readinInputSettings()
{
    QVariant var = SAKSettings::instance()->value(mSettingStringInputModel);
    int index = 0;
    if (var.isNull()){
        index = 4;
    }else{
        index = var.toInt();
    }
    mInputModelComboBox->setCurrentIndex(index);

    var = SAKSettings::instance()->value(mSettingStringCycleTime);
    QString cycleTime;
    if (var.isNull()){
        cycleTime = QString("1000");
    }else{
        cycleTime = var.toString();
    }
    mCycleTimeLineEdit->setText(cycleTime);

    bool value = SAKSettings::instance()->value(mSettingStringAddCRC).toBool();
    mAddCRCCheckBox->setChecked(value);

    index = SAKSettings::instance()->value(mSettingStringcrcParameterModel).toInt();
    mCrcParameterModelsComboBox->setCurrentIndex(index);
}

void SAKDebugPage::readinOutputSettings()
{
    // 某些设置默认为勾选
    auto setValue = [](QVariant &var){
        if (var.isNull()){
            return true;
        }else{
            return var.toBool();
        }
    };

    QVariant var = SAKSettings::instance()->value(mSettingStringOutputTextFormat);
    int index = 0;
    if (var.isNull()){
        index = 4;
    }else{
        index = var.toInt();
    }
    mOutputTextFormatComboBox->setCurrentIndex(index);

    var = SAKSettings::instance()->value(mSettingStringShowDate);
    bool value = SAKSettings::instance()->value(mSettingStringShowDate).toBool();
    mShowDateCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(mSettingStringAutoWrap);
    value = setValue(var);
    mAutoWrapCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(mSettingStringShowTime).toBool();
    mShowTimeCheckBox->setChecked(value);

    value = SAKSettings::instance()->value(mSettingStringShowMs).toBool();
    mShowMsCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(mSettingStringShowRx);
    value = setValue(var);
    mShowRxDataCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(mSettingStringShowTx);
    value = setValue(var);
    mShowTxDataCheckBox->setChecked(value);
}

void SAKDebugPage::cleanInfo()
{
    mClearInfoTimer.stop();
    mInfoLabel->clear();
}

void SAKDebugPage::openOrColoseDevice()
{
    mSwitchPushButton->setEnabled(false);
    if (!mDevice){
        setupDevice();
    }

    if (mDevice){
        if (mDevice->isRunning()){
            closeDevice();
        }else{
            openDevice();
        }
    }
    mSwitchPushButton->setEnabled(true);
}

void SAKDebugPage::openDevice()
{
    if (mDevice){
        mDevice->start();
        mSwitchPushButton->setText(tr("关闭设备"));
    }
}

void SAKDebugPage::closeDevice()
{
    if (mDevice){
        mDevice->requestInterruption();
        mDevice->wakeMe();
        mDevice->exit();
        mDevice->wait();
        mDevice->deleteLater();
        mDevice = Q_NULLPTR;
        mSwitchPushButton->setText(tr("打开设备"));
    }
}

void SAKDebugPage::setupDevice()
{
    mDevice = createDevice();
    if (mDevice){
        connect(this, &SAKDebugPage::requestWriteData, mDevice, &SAKDebugPageDevice::writeBytes);
        connect(mDevice, &SAKDebugPageDevice::bytesWritten, this, &SAKDebugPage::bytesWritten);
#if 0
        connect(device, &SAKDevice::bytesRead, this, &SAKDebugPage::bytesRead);
#else
        /// @brief 设备读取到的数据传输至协议分析器中，分析完成的数据回传至调试页面中
        SAKMoreSettingsWidget *moreSettingsWidget = mOtherSettings->moreSettingsWidget();
        SAKProtocolAnalyzerWidget *protocolAnalyzerWidget = moreSettingsWidget->protocolAnalyzerWidget();
        connect(mDevice, &SAKDebugPageDevice::bytesRead, protocolAnalyzerWidget, &SAKProtocolAnalyzerWidget::inputBytes);
        connect(protocolAnalyzerWidget, &SAKProtocolAnalyzerWidget::bytesAnalysed, this, &SAKDebugPage::bytesRead);
#endif
        connect(mDevice, &SAKDebugPageDevice::messageChanged, this, &SAKDebugPage::outputMessage);
        connect(mDevice, &SAKDebugPageDevice::deviceStateChanged, this, &SAKDebugPage::changedDeviceState);
        connect(mDevice, &SAKDebugPageDevice::finished, this, &SAKDebugPage::closeDevice);
    }
}

void SAKDebugPage::setupController()
{
    QWidget *controller = controllerWidget();
    if (controller){
        QHBoxLayout *layout = new QHBoxLayout(mDeviceSettingFrame);
        mDeviceSettingFrame->setLayout(layout);
        layout->addWidget(controller);

        /// @brief qt5.13及以上版本setMargin()接口改为setContentsMargins()
#if (QT_VERSION >= QT_VERSION_CHECK(5,13,0))
        layout->setContentsMargins(0, 0, 0, 0);
#else
        layout->setMargin(0);
#endif
    }
}

void SAKDebugPage::on_refreshPushButton_clicked()
{
    refreshDevice();
}

void SAKDebugPage::on_switchPushButton_clicked()
{
    openOrColoseDevice();
}

void SAKDebugPage::on_inputModelComboBox_currentIndexChanged(int index)
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringInputModel, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_cycleTimeLineEdit_textChanged(const QString &text)
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringCycleTime, QVariant::fromValue(text));
    }
}

void SAKDebugPage::on_addCRCCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringAddCRC, QVariant::fromValue(mAddCRCCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_crcSettingsPushButton_clicked()
{
    mDebugPageInputManager->showCrcSettingsDialog();
}

void SAKDebugPage::on_crcParameterModelsComboBox_currentIndexChanged(int index)
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringcrcParameterModel, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_outputTextFormatComboBox_currentIndexChanged(int index)
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringOutputTextFormat, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_showDateCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowDate, QVariant::fromValue(mShowDateCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_autoWrapCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringAutoWrap, QVariant::fromValue(mAutoWrapCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showTimeCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowTime, QVariant::fromValue(mShowTimeCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showMsCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowMs, QVariant::fromValue(mShowMsCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showRxDataCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowRx, QVariant::fromValue(mShowRxDataCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showTxDataCheckBox_clicked()
{
    if (!mIsInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowTx, QVariant::fromValue(mShowTxDataCheckBox->isChecked()));
    }
}

void SAKDebugPage::initUiPointer()
{
    /// @brief 设备管理组
    mSwitchPushButton        = mUi->switchPushButton;
    mRefreshPushButton       = mUi->refreshPushButton;
    mDeviceSettingFrame      = mUi->deviceSettingFrame;

    /// @brief 消息输出组
    mInfoLabel               = mUi->infoLabel;

    /// @brief 输入设置组
    mInputModelComboBox      = mUi->inputModelComboBox;
    mCycleEnableCheckBox     = mUi->cycleEnableCheckBox;
    mCycleTimeLineEdit       = mUi->cycleTimeLineEdit;
    mSaveInputDataPushButton = mUi->saveInputDataPushButton;
    mReadinFilePushButton    = mUi->readinFilePushButton;
    mAddCRCCheckBox          = mUi->addCRCCheckBox;
    mCrcSettingsPushButton   = mUi->crcSettingsPushButton;
    mClearInputPushButton    = mUi->clearInputPushButton;
    mSendPushButton          = mUi->sendPushButton;
    mInputTextEdit           = mUi->inputTextEdit;
    mCrcParameterModelsComboBox = mUi->crcParameterModelsComboBox;
    mCrcLabel                = mUi->crcLabel;
    mPresetPushButton        = mUi->presetPushButton;
    mSendPresetPushButton    = mUi->sendPresetPushButton;

    /// @brief 输出设置组
    mRxLabel                 = mUi->rxLabel;
    mTxLabel                 = mUi->txLabel;
    mOutputTextFormatComboBox= mUi->outputTextFormatComboBox;
    mAutoWrapCheckBox        = mUi->autoWrapCheckBox;
    mShowDateCheckBox        = mUi->showDateCheckBox;
    mShowTimeCheckBox        = mUi->showTimeCheckBox;
    mShowMsCheckBox          = mUi->showMsCheckBox;
    mShowRxDataCheckBox      = mUi->showRxDataCheckBox;
    mShowTxDataCheckBox      = mUi->showTxDataCheckBox;
    mSaveOutputToFileCheckBox = mUi->saveOutputToFileCheckBox;
    mOutputFilePathPushButton= mUi->outputFilePathPushButton;
    mClearOutputPushButton   = mUi->clearOutputPushButton;
    mSaveOutputPushButton    = mUi->saveOutputPushButton;
    mOutputTextBroswer       = mUi->outputTextBroswer;

    /// @brief 数据统计
    mRxSpeedLabel            = mUi->rxSpeedLabel;
    mTxSpeedLabel            = mUi->txSpeedLabel;
    mRxFramesLabel           = mUi->rxFramesLabel;
    mTxFramesLabel           = mUi->txFramesLabel;
    mRxBytesLabel            = mUi->rxBytesLabel;
    mTxBytesLabel            = mUi->txBytesLabel;
    mResetTxCountPushButton  = mUi->resetTxCountPushButton;
    mResetRxCountPushButton  = mUi->resetRxCountPushButton;

    /// @brief 其他设置
    mTransmissionSettingPushButton = mUi->transmissionSettingPushButton;
    mReadWriteSettingPushButton    = mUi->readWriteSettingPushButton;
    mAutoResponseSettingPushButton = mUi->autoResponseSettingPushButton;
    mTimingSendingPushButton       = mUi->timingSendingPushButton;
    mHighlightSettingPushButton    = mUi->highlightSettingPushButton;
    mMoreSettingsPushButton        = mUi->moreSettingsPushButton;

    /// @brief 数据可视化
    mDataVisualizationPushButton = mUi->dataVisualizationPushButton;
}

void SAKDebugPage::on_dataVisualizationPushButton_clicked()
{
#ifdef SAK_IMPORT_CHARTS_MODULE
    if (mDataVisualizationManager){
        if (mDataVisualizationManager->isHidden()){
            mDataVisualizationManager->show();
        }else{
            mDataVisualizationManager->activateWindow();
        }
    }else{
        mDataVisualizationManager = new SAKChartsController(this);
        mDataVisualizationManager->show();
        connect(mDataVisualizationManager, &SAKChartsController::destroyed, [&](){
            mDataVisualizationManager = Q_NULLPTR;
        });
    }
#else
    QMessageBox::warning(this, tr("不支持的功能"), tr("该功能已被开发者禁用，可能原因是该平台不支持该功能"));
#endif
}
