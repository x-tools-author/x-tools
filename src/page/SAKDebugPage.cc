/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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

#include "SAKGlobal.hh"
#include "SAKSettings.hh"
#include "SAKDebugPage.hh"
#include "SAKCRCInterface.hh"
#include "SAKStatisticsManager.hh"
#include "SAKOtherSettingsManager.hh"
#include "SAKDebugPageInputManager.hh"
#include "SAKDebugPageOutputManager.hh"
#include "SAKHighlightSettingsWidget.hh"
#ifdef SAK_IMPORT_CHARTS_MODULE
#include "SAKDataVisualizationManager.hh"
#endif

#include "ui_SAKDebugPage.h"

#define MINI_READ_WRITE_WATINGT_TIME 5   // 读写等待最小时间(单位为：ms)

SAKDebugPage::SAKDebugPage(int type, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKDebugPage)
    ,debugPageType (type)
{
    isInitializing = true;
    initSettingString();

    ui->setupUi(this);
    initUiPointer();

    outputManager           = new SAKDebugPageOutputManager(this, this);
    otherSettings           = new SAKOtherSettingsManager(this, this);
    statisticsManager       = new SAKStatisticsManager(this, this);
    debugPageInputManager   = new SAKDebugPageInputManager(this, this);
#ifdef SAK_IMPORT_CHARTS_MODULE
    dataVisualizationManager= nullptr;
#endif

    _readWriteParameters.waitForReadyReadTime = MINI_READ_WRITE_WATINGT_TIME;
    _readWriteParameters.waitForBytesWrittenTime = MINI_READ_WRITE_WATINGT_TIME;

    connect(this, &SAKDebugPage::deviceStatusChanged, this, &SAKDebugPage::changedDeviceStatus);
    resize(800, 600);

    clearInfoTimer.setInterval(8*1000);
    connect(&clearInfoTimer, &QTimer::timeout, this, &SAKDebugPage::cleanInfo);

    readinSettings();
    isInitializing = false;
}

SAKDebugPage::~SAKDebugPage()
{
    delete ui;
#ifdef SAK_IMPORT_CHARTS_MODULE
    delete dataVisualizationManager;
#endif
}

void SAKDebugPage::write(QByteArray data)
{
    emit writeDataRequest(data);
}

void SAKDebugPage::writeRawData(QString rawData, int textFormat)
{
    emit writeRawDataRequest(rawData, textFormat);
}

void SAKDebugPage::outputMessage(QString msg, bool isInfo)
{
    QString time = QDateTime::currentDateTime().toString("hh:mm:ss ");
    QString temp;
    temp.append(time);
    temp.append(msg);
    time = QString("<font color=silver>%1</font>").arg(time);

    if (isInfo){
        infoLabel->setStyleSheet(QString("QLabel{color:black}"));
    }else{
        infoLabel->setStyleSheet(QString("QLabel{color:red}"));
        QApplication::beep();
    }

    msg.prepend(time);
    infoLabel->setText(msg);
    clearInfoTimer.start();
}

struct SAKDebugPage::ReadWriteParameters SAKDebugPage::readWriteParameters()
{
    ReadWriteParameters parameters;
    readWriteParametersQMutex.lock();    
    parameters.waitForReadyReadTime = _readWriteParameters.waitForReadyReadTime;
    parameters.waitForBytesWrittenTime = _readWriteParameters.waitForBytesWrittenTime;
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


void SAKDebugPage::openOrColoseDevice()
{

}

void SAKDebugPage::refreshDevice()
{

}

QWidget *SAKDebugPage::controllerWidget()
{
    return nullptr;
}

void SAKDebugPage::setUpController()
{
    QWidget *controller = controllerWidget();
    if (controller){
        QHBoxLayout *layout = new QHBoxLayout(deviceSettingFrame);
        deviceSettingFrame->setLayout(layout);
        layout->addWidget(controller);

        /*
         * qt 5.13及 以上版本setMargin()接口改为setContentsMargins()
         */
#if (QT_VERSION >= QT_VERSION_CHECK(5,13,0))
        layout->setContentsMargins(0, 0, 0, 0);
#else
        layout->setMargin(0);
#endif
    }
}

void SAKDebugPage::changedDeviceStatus(bool opened)
{
    sendPushButton->setEnabled(opened);
    sendPresetPushButton->setEnabled(opened);
    cycleEnableCheckBox->setEnabled(opened);
}

void SAKDebugPage::cleanInfo()
{
    clearInfoTimer.stop();
    infoLabel->clear();
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
        SAKSettings::instance()->setValue(settingStringInputModel, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_cycleTimeLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringCycleTime, QVariant::fromValue(text));
    }
}

void SAKDebugPage::on_addCRCCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringAddCRC, QVariant::fromValue(addCRCCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_bigeEndianCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringBigeEndian, QVariant::fromValue(bigeEndianCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_crcParameterModelsComboBox_currentIndexChanged(int index)
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringcrcParameterModel, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_outputTextFormatComboBox_currentIndexChanged(int index)
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringOutputTextFormat, QVariant::fromValue(index));
    }
}

void SAKDebugPage::on_showDateCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringShowDate, QVariant::fromValue(showDateCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_autoWrapCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringAutoWrap, QVariant::fromValue(autoWrapCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showTimeCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringShowTime, QVariant::fromValue(showTimeCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showMsCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringShowMs, QVariant::fromValue(showMsCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showRxDataCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringShowRx, QVariant::fromValue(showRxDataCheckBox->isChecked()));
    }
}

void SAKDebugPage::on_showTxDataCheckBox_clicked()
{
    if (!isInitializing){
        SAKSettings::instance()->setValue(settingStringShowTx, QVariant::fromValue(showTxDataCheckBox->isChecked()));
    }
}

void SAKDebugPage::initUiPointer()
{
    /*
     * 设备管理组
     */
    switchPushButton        = ui->switchPushButton;
    refreshPushButton       = ui->refreshPushButton;
    deviceSettingFrame      = ui->deviceSettingFrame;

    /*
     * 消息输出组
     */
    infoLabel               = ui->infoLabel;

    /*
     * 输入设置组
     */
    inputModelComboBox      = ui->inputModelComboBox;
    cycleEnableCheckBox     = ui->cycleEnableCheckBox;
    cycleTimeLineEdit       = ui->cycleTimeLineEdit;
    saveInputDataPushButton = ui->saveInputDataPushButton;
    readinFilePushButton    = ui->readinFilePushButton;
    addCRCCheckBox          = ui->addCRCCheckBox;
    bigeEndianCheckBox      = ui->bigeEndianCheckBox;
    clearInputPushButton    = ui->clearInputPushButton;
    sendPushButton          = ui->sendPushButton;
    inputTextEdit           = ui->inputTextEdit;
    crcParameterModelsComboBox = ui->crcParameterModelsComboBox;
    crcLabel                = ui->crcLabel;
    presetPushButton        = ui->presetPushButton;
    sendPresetPushButton    = ui->sendPresetPushButton;

    /*
     * 输出设置组
     */
    rxLabel                 = ui->rxLabel;
    txLabel                 = ui->txLabel;
    outputTextFormatComboBox= ui->outputTextFormatComboBox;
    autoWrapCheckBox        = ui->autoWrapCheckBox;
    showDateCheckBox        = ui->showDateCheckBox;
    showTimeCheckBox        = ui->showTimeCheckBox;
    showMsCheckBox          = ui->showMsCheckBox;
    showRxDataCheckBox      = ui->showRxDataCheckBox;
    showTxDataCheckBox      = ui->showTxDataCheckBox;
    saveOutputFileToFilecheckBox = ui->saveOutputFileToFilecheckBox;
    outputFilePathPushButton= ui->outputFilePathPushButton;
    clearOutputPushButton   = ui->clearOutputPushButton;
    saveOutputPushButton    = ui->saveOutputPushButton;
    outputTextBroswer       = ui->outputTextBroswer;


    /*
     * 数据统计
     */
    rxSpeedLabel            = ui->rxSpeedLabel;
    txSpeedLabel            = ui->txSpeedLabel;
    rxFramesLabel           = ui->rxFramesLabel;
    txFramesLabel           = ui->txFramesLabel;
    rxBytesLabel            = ui->rxBytesLabel;
    txBytesLabel            = ui->txBytesLabel;
    resetTxCountPushButton  = ui->resetTxCountPushButton;
    resetRxCountPushButton  = ui->resetRxCountPushButton;

    /*
     * 其他设置
     */
    transmissionSettingPushButton = ui->transmissionSettingPushButton;
    readWriteSettingPushButton = ui->readWriteSettingPushButton;
    autoResponseSettingPushButton = ui->autoResponseSettingPushButton;    
    timingSendingPushButton = ui->timingSendingPushButton;
    highlightSettingPushButton = ui->highlightSettingPushButton;
    moreSettingsPushButton = ui->moreSettingsPushButton;    

    /*
     * 数据可视化
     */
    dataVisualizationPushButton = ui->dataVisualizationPushButton;
}

void SAKDebugPage::on_dataVisualizationPushButton_clicked()
{
#ifdef SAK_IMPORT_CHARTS_MODULE
    if (dataVisualizationManager){
        if (dataVisualizationManager->isHidden()){
            dataVisualizationManager->show();
        }else{
            dataVisualizationManager->activateWindow();
        }
    }else{
        dataVisualizationManager = new SAKDataVisualizationManager(this);
        dataVisualizationManager->show();
        connect(dataVisualizationManager, &SAKDataVisualizationManager::destroyed, [&](){
            dataVisualizationManager = nullptr;
        });
    }
#else
    QMessageBox::warning(this, tr("不支持的功能"), tr("该功能已被开发者禁用，可能原因是该平台不支持该功能"));
#endif
}

void SAKDebugPage::initSettingKey()
{
    switch (debugPageType) {
#ifdef SAK_IMPORT_COM_MODULE
    case SAKGlobal::SAKEnumDebugPageTypeCOM:
        settingKey = QString("COM");
        break;
#endif
    case SAKGlobal::SAKEnumDebugPageTypeUDP:
        settingKey = QString("UDP");
        break;
    case SAKGlobal::SAKEnumDebugPageTypeTCPClient:
        settingKey = QString("TCPClient");
        break;
    case SAKGlobal::SAKEnumDebugPageTypeTCPServer:
        settingKey = QString("TCPServer");
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
    settingStringInputModel        = QString("%1/inputModel").arg(settingKey);
    settingStringCycleTime         = QString("%1/cycleTime").arg(settingKey);
    settingStringAddCRC            = QString("%1/addCRC").arg(settingKey);
    settingStringBigeEndian        = QString("%1/bigeEndian").arg(settingKey);
    settingStringcrcParameterModel = QString("%1/parameterModel").arg(settingKey);
}

void SAKDebugPage::initOutputSettingString()
{
    settingStringOutputTextFormat = QString("%1/outputTextFormat").arg(settingKey);
    settingStringShowDate    = QString("%1/showDate").arg(settingKey);
    settingStringAutoWrap    = QString("%1/autoWrap").arg(settingKey);
    settingStringShowTime    = QString("%1/showTime").arg(settingKey);
    settingStringShowMs      = QString("%1/showMs").arg(settingKey);
    settingStringShowRx      = QString("%1/showRx").arg(settingKey);
    settingStringShowTx      = QString("%1/showTx").arg(settingKey);
}

void SAKDebugPage::readinSettings()
{
    readinInputSettings();
    readinOutputSettings();
}

void SAKDebugPage::readinInputSettings()
{
    QVariant var = SAKSettings::instance()->value(settingStringInputModel);
    int index = 0;
    if (var.isNull()){
        index = 4;
    }else{
        index = var.toInt();
    }
    inputModelComboBox->setCurrentIndex(index);

    var = SAKSettings::instance()->value(settingStringCycleTime);
    QString cycleTime;
    if (var.isNull()){
        cycleTime = QString("1000");
    }else{
         cycleTime = var.toString();
    }
    cycleTimeLineEdit->setText(cycleTime);

    bool value = SAKSettings::instance()->value(settingStringAddCRC).toBool();
    addCRCCheckBox->setChecked(value);

    value = SAKSettings::instance()->value(settingStringBigeEndian).toBool();
    bigeEndianCheckBox->setChecked(value);

    index = SAKSettings::instance()->value(settingStringcrcParameterModel).toInt();
    crcParameterModelsComboBox->setCurrentIndex(index);
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

    QVariant var = SAKSettings::instance()->value(settingStringOutputTextFormat);
    int index = 0;
    if (var.isNull()){
        index = 4;
    }else{
        index = var.toInt();
    }
    outputTextFormatComboBox->setCurrentIndex(index);

    var = SAKSettings::instance()->value(settingStringShowDate);
    bool value = SAKSettings::instance()->value(settingStringShowDate).toBool();
    showDateCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(settingStringAutoWrap);
    value = setValue(var);
    autoWrapCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(settingStringShowTime).toBool();
    showTimeCheckBox->setChecked(value);

    value = SAKSettings::instance()->value(settingStringShowMs).toBool();
    showMsCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(settingStringShowRx);
    value = setValue(var);
    showRxDataCheckBox->setChecked(value);

    var = SAKSettings::instance()->value(settingStringShowTx);
    value = setValue(var);
    showTxDataCheckBox->setChecked(value);
}
