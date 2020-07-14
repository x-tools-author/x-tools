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

#include "SAKDevice.hh"
#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKProtocolAnalyzer.hh"
#include "SAKStatisticsManager.hh"
#include "SAKMoreSettingsWidget.hh"
#include "SAKOtherSettingsManager.hh"
#include "SAKDebugPageInputManager.hh"
#include "SAKDebugPageOutputManager.hh"
#include "SAKProtocolAnalyzerWidget.hh"
#include "SAKHighlightSettingsWidget.hh"
#ifdef SAK_IMPORT_CHARTS_MODULE
#include "SAKChartsManager.hh"
#endif
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKDebugPage.h"

#define MINI_READ_WRITE_WATINGT_TIME 5   // 读写等待最小时间(单位为：ms)

SAKDebugPage::SAKDebugPage(int type, QWidget *parent)
    :QWidget(parent)
    ,device(Q_NULLPTR)    
    ,debugPageType(type)
    ,ui(new Ui::SAKDebugPage)
{
    databaseInterface = SAKDebugPageDatabaseInterface::instance();

    isInitializing = true;
    initSettingString();

    ui->setupUi(this);
    initUiPointer();

    mOutputManager           = new SAKDebugPageOutputManager(this, this);
    mOtherSettings           = new SAKOtherSettingsManager(this, this);
    mStatisticsManager       = new SAKStatisticsManager(this, this);
    mDebugPageInputManager   = new SAKDebugPageInputManager(this, this);
#ifdef SAK_IMPORT_CHARTS_MODULE
    mDataVisualizationManager= Q_NULLPTR;
#endif

    _readWriteParameters.waitForReadyReadTime = MINI_READ_WRITE_WATINGT_TIME;
    _readWriteParameters.waitForBytesWrittenTime = MINI_READ_WRITE_WATINGT_TIME;
    _readWriteParameters.runIntervalTime = 25;
#if 0
    resize(800, 600);
#endif
    clearInfoTimer.setInterval(8*1000);
    connect(&clearInfoTimer, &QTimer::timeout, this, &SAKDebugPage::cleanInfo);

    readinSettings();
    isInitializing = false;
}

SAKDebugPage::~SAKDebugPage()
{
    if (device){
        device->requestInterruption();
        device->wakeMe();
        device->exit();
        device->wait();
        delete device;
        device = Q_NULLPTR;
    }

    if (ui){
        delete ui;
        ui = Q_NULLPTR;
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
    clearInfoTimer.start();
}

struct SAKDebugPage::ReadWriteParameters SAKDebugPage::readWriteParameters()
{
    ReadWriteParameters parameters;
    readWriteParametersQMutex.lock();
    parameters.waitForReadyReadTime = _readWriteParameters.waitForReadyReadTime;
    parameters.waitForBytesWrittenTime = _readWriteParameters.waitForBytesWrittenTime;
    parameters.runIntervalTime = _readWriteParameters.runIntervalTime;
    readWriteParametersQMutex.unlock();

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

    readWriteParametersQMutex.lock();
    _readWriteParameters.waitForReadyReadTime = parameters.waitForReadyReadTime;
    _readWriteParameters.waitForBytesWrittenTime = parameters.waitForBytesWrittenTime;
    readWriteParametersQMutex.unlock();
}

SAKDebugPageDatabaseInterface *SAKDebugPage::databaseInterfaceInstance()
{
   return databaseInterface;
}

quint32 SAKDebugPage::pageType()
{
    return debugPageType;
}

void SAKDebugPage::refreshDevice()
{

}

QWidget *SAKDebugPage::controllerWidget()
{
    return Q_NULLPTR;
}

SAKDevice *SAKDebugPage::createDevice()
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
    switch (debugPageType) {
#ifdef SAK_IMPORT_COM_MODULE
    case SAKDataStruct::DebugPageTypeCOM:
        settingKey = QString("COM");
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
        settingKey = QString("UDP");
        break;
    case SAKDataStruct::DebugPageTypeTCPClient:
        settingKey = QString("TCPC");
        break;
    case SAKDataStruct::DebugPageTypeTCPServer:
        settingKey = QString("TCPS");
        break;
    case SAKDataStruct::DebugPageTypeWebSocketClient:
        settingKey = QString("WEBSOCKETC");
        break;
    case SAKDataStruct::DebugPageTypeWebSocketServer:
        settingKey = QString("WEBSOCKETS");
        break;
    default:
        settingKey = QString("UnknowDebugPage");
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
    mSettingStringInputModel        = QString("%1/inputModel").arg(settingKey);
    mSettingStringCycleTime         = QString("%1/cycleTime").arg(settingKey);
    mSettingStringAddCRC            = QString("%1/addCRC").arg(settingKey);
    mSettingStringBigeEndian        = QString("%1/bigeEndian").arg(settingKey);
    mSettingStringcrcParameterModel = QString("%1/parameterModel").arg(settingKey);
}

void SAKDebugPage::initOutputSettingString()
{
    mSettingStringOutputTextFormat = QString("%1/outputTextFormat").arg(settingKey);
    mSettingStringShowDate    = QString("%1/showDate").arg(settingKey);
    mSettingStringAutoWrap    = QString("%1/autoWrap").arg(settingKey);
    mSettingStringShowTime    = QString("%1/showTime").arg(settingKey);
    mSettingStringShowMs      = QString("%1/showMs").arg(settingKey);
    mSettingStringShowRx      = QString("%1/showRx").arg(settingKey);
    mSettingStringShowTx      = QString("%1/showTx").arg(settingKey);
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
    clearInfoTimer.stop();
    mInfoLabel->clear();
}

void SAKDebugPage::openOrColoseDevice()
{
    mSwitchPushButton->setEnabled(false);
    if (!device){
        setupDevice();
    }

    if (device){
        if (device->isRunning()){
            closeDevice();
        }else{
            openDevice();
        }
    }
    mSwitchPushButton->setEnabled(true);
}

void SAKDebugPage::openDevice()
{
    if (device){
        device->start();
        mSwitchPushButton->setText(tr("关闭设备"));
    }
}

void SAKDebugPage::closeDevice()
{
    if (device){
        device->requestInterruption();
        device->wakeMe();
        device->exit();
        device->wait();
        device->deleteLater();
        device = Q_NULLPTR;
        mSwitchPushButton->setText(tr("打开设备"));
    }
}

void SAKDebugPage::setupDevice()
{
    device = createDevice();
    if (device){
        connect(this, &SAKDebugPage::requestWriteData, device, &SAKDevice::writeBytes);
        connect(device, &SAKDevice::bytesWritten, this, &SAKDebugPage::bytesWritten);
#if 0
        connect(device, &SAKDevice::bytesRead, this, &SAKDebugPage::bytesRead);
#else
        /// @brief 设备读取到的数据传输至协议分析器中，分析完成的数据回传至调试页面中
        SAKMoreSettingsWidget *moreSettingsWidget = mOtherSettings->moreSettingsWidget();
        SAKProtocolAnalyzerWidget *protocolAnalyzerWidget = moreSettingsWidget->protocolAnalyzerWidget();
        connect(device, &SAKDevice::bytesRead, protocolAnalyzerWidget, &SAKProtocolAnalyzerWidget::inputBytes);
        connect(protocolAnalyzerWidget, &SAKProtocolAnalyzerWidget::bytesAnalysed, this, &SAKDebugPage::bytesRead);
#endif
        connect(device, &SAKDevice::messageChanged, this, &SAKDebugPage::outputMessage);
        connect(device, &SAKDevice::deviceStateChanged, this, &SAKDebugPage::changedDeviceState);
        connect(device, &SAKDevice::finished, this, &SAKDebugPage::closeDevice);
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
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringInputModel, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_cycleTimeLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringCycleTime, QVariant::fromValue(text));
    }
}

void SAKDebugPage::on_addCRCCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringAddCRC, QVariant::fromValue(mAddCRCCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_crcSettingsPushButton_clicked()
{
    mDebugPageInputManager->showCrcSettingsDialog();
}

void SAKDebugPage::on_crcParameterModelsComboBox_currentIndexChanged(int index)
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringcrcParameterModel, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_outputTextFormatComboBox_currentIndexChanged(int index)
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringOutputTextFormat, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_showDateCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowDate, QVariant::fromValue(mShowDateCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_autoWrapCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringAutoWrap, QVariant::fromValue(mAutoWrapCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showTimeCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowTime, QVariant::fromValue(mShowTimeCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showMsCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowMs, QVariant::fromValue(mShowMsCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showRxDataCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowRx, QVariant::fromValue(mShowRxDataCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showTxDataCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(mSettingStringShowTx, QVariant::fromValue(mShowTxDataCheckBox->isChecked()));
    }
}

void SAKDebugPage::initUiPointer()
{
    /// @brief 设备管理组
    mSwitchPushButton        = ui->switchPushButton;
    mRefreshPushButton       = ui->refreshPushButton;
    mDeviceSettingFrame      = ui->deviceSettingFrame;

    /// @brief 消息输出组
    mInfoLabel               = ui->infoLabel;

    /// @brief 输入设置组
    mInputModelComboBox      = ui->inputModelComboBox;
    mCycleEnableCheckBox     = ui->cycleEnableCheckBox;
    mCycleTimeLineEdit       = ui->cycleTimeLineEdit;
    mSaveInputDataPushButton = ui->saveInputDataPushButton;
    mReadinFilePushButton    = ui->readinFilePushButton;
    mAddCRCCheckBox          = ui->addCRCCheckBox;
    mCrcSettingsPushButton   = ui->crcSettingsPushButton;
    mClearInputPushButton    = ui->clearInputPushButton;
    mSendPushButton          = ui->sendPushButton;
    mInputTextEdit           = ui->inputTextEdit;
    mCrcParameterModelsComboBox = ui->crcParameterModelsComboBox;
    mCrcLabel                = ui->crcLabel;
    mPresetPushButton        = ui->presetPushButton;
    mSendPresetPushButton    = ui->sendPresetPushButton;

    /// @brief 输出设置组
    mRxLabel                 = ui->rxLabel;
    mTxLabel                 = ui->txLabel;
    mOutputTextFormatComboBox= ui->outputTextFormatComboBox;
    mAutoWrapCheckBox        = ui->autoWrapCheckBox;
    mShowDateCheckBox        = ui->showDateCheckBox;
    mShowTimeCheckBox        = ui->showTimeCheckBox;
    mShowMsCheckBox          = ui->showMsCheckBox;
    mShowRxDataCheckBox      = ui->showRxDataCheckBox;
    mShowTxDataCheckBox      = ui->showTxDataCheckBox;
    mSaveOutputFileToFilecheckBox = ui->saveOutputFileToFilecheckBox;
    mOutputFilePathPushButton= ui->outputFilePathPushButton;
    mClearOutputPushButton   = ui->clearOutputPushButton;
    mSaveOutputPushButton    = ui->saveOutputPushButton;
    mOutputTextBroswer       = ui->outputTextBroswer;

    /// @brief 数据统计
    mRxSpeedLabel            = ui->rxSpeedLabel;
    mTxSpeedLabel            = ui->txSpeedLabel;
    mRxFramesLabel           = ui->rxFramesLabel;
    mTxFramesLabel           = ui->txFramesLabel;
    mRxBytesLabel            = ui->rxBytesLabel;
    mTxBytesLabel            = ui->txBytesLabel;
    mResetTxCountPushButton  = ui->resetTxCountPushButton;
    mResetRxCountPushButton  = ui->resetRxCountPushButton;

    /// @brief 其他设置
    mTransmissionSettingPushButton = ui->transmissionSettingPushButton;
    mReadWriteSettingPushButton    = ui->readWriteSettingPushButton;
    mAutoResponseSettingPushButton = ui->autoResponseSettingPushButton;
    mTimingSendingPushButton       = ui->timingSendingPushButton;
    mHighlightSettingPushButton    = ui->highlightSettingPushButton;
    mMoreSettingsPushButton        = ui->moreSettingsPushButton;

    /// @brief 数据可视化
    mDataVisualizationPushButton = ui->dataVisualizationPushButton;
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
        mDataVisualizationManager = new SAKChartsManager(this);
        mDataVisualizationManager->show();
        connect(mDataVisualizationManager, &SAKChartsManager::destroyed, [&](){
            mDataVisualizationManager = Q_NULLPTR;
        });
    }
#else
    QMessageBox::warning(this, tr("不支持的功能"), tr("该功能已被开发者禁用，可能原因是该平台不支持该功能"));
#endif
}
