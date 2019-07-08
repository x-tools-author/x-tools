/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include <QDebug>
#include <QPixmap>
#include <QDateTime>
#include <QSettings>
#include <QKeyEvent>
#include <QMetaEnum>
#include <QScrollBar>
#include <QGridLayout>
#include <QFileDialog>
#include <QTextStream>
#include <QIntValidator>
#include <QLoggingCategory>

#include "SAKDebugPage.hh"
#include "ui_SAKDebugPage.h"
#include "SAKDataFactory.hh"
#include "SAKCRCInterface.hh"
#include "SAKHighlighterSettingPanel.hh"

SAKDebugPage::SAKDebugPage(QWidget *parent)
    :QWidget(parent)
    ,crcInterface (new SAKCRCInterface(this)) 
    ,ui (new Ui::SAKDebugPage)
{
    /*
     * 安装ui
     */
    ui->setupUi(this);

    /*
     * 初始化ui指针变量
     */
    initUiPointer();

    /*
     * 注册数据类型
     */
    registerMetaType();

    /*
     * 数据传输关联
     */
    dataFactory = new SAKDataFactory(this);
    connect(this, &SAKDebugPage::sendRawData, dataFactory, &SAKDataFactory::handleTheDataThatNeedsToBeSent);
    connect(this, &SAKDebugPage::dataReadOrwritten, dataFactory, &SAKDataFactory::handleTheDataThatNeedsToBeOutputted);
    connect(dataFactory, &SAKDataFactory::sendBytes, this, &SAKDebugPage::writeBytes);
    connect(dataFactory, &SAKDataFactory::outputData, this, &SAKDebugPage::outputData);
    dataFactory->start();

    /*
     * 初始化输入输出模式
     */
    inputModelComboBox->clear();
    outputModelComboBox->clear();
    QMetaEnum textModel = QMetaEnum::fromType<SAKDebugPage::TextDisplayModel>();
    for (int i = 0; i < textModel.keyCount(); i++){
        inputModelComboBox->addItem(QString(textModel.valueToKey(i)));
        outputModelComboBox->addItem(QString(textModel.valueToKey(i)));
    }

    /*
     * 初始化ui显示
     */
    initUI();

    /*
     * 信号关联
     */
    connect(this, &SAKDebugPage::deviceStatusChanged, this, &SAKDebugPage::changedDeviceStatus);

    /*
     * 初始化参数
     */
    inputParameters.sendCircularly = false;
    inputParameters.addCRC = false;
    inputParameters.bigEnfian = false;
    inputParameters.cycleTime = false;
    inputParameters.inputModel = TextDisplayModel::Ascii;
    inputParameters.crcModel = SAKCRCInterface::CRC_8;
}

SAKDebugPage::~SAKDebugPage()
{
    delete ui;
    delete logCategory;
}

unsigned long SAKDebugPage::readDelayTimes()
{
    return 0;
}

unsigned long SAKDebugPage::writeDelayTimes()
{
    return 0;
}

void SAKDebugPage::setUpController()
{
    QWidget *controller = controllerWidget();
    if (controller){
        QGridLayout *layout = reinterpret_cast<QGridLayout *>(deviceSettingGroupBox->layout());
        layout->addWidget(controller, 2, 0, 1, 2);
    }
}

SAKDebugPage::OutputParameters SAKDebugPage::outputParameters()
{
    OutputParameters parameters;
    parameters.showMS = showMsCheckBox->isChecked();
    parameters.showDate = showDateCheckBox->isChecked();
    parameters.showTime = showTimeCheckBox->isChecked();

    QMetaEnum outputTextModelTemp = QMetaEnum::fromType<TextDisplayModel>();
    bool ok = false;
    parameters.textModel = static_cast<TextDisplayModel>(outputTextModelTemp.keyToValue(outputModelComboBox->currentText().toLatin1().data(), &ok));
    if (!ok){
        parameters.textModel = SAKDebugPage::Hex;
    }

    return parameters;
}

void SAKDebugPage::setLabelText(QLabel *label, quint64 text)
{
    QString str = QString::number(text);
    label->setText(str);
}

void SAKDebugPage::formattingInputText(TextDisplayModel model)
{
    QString plaintext = inputTextEdit->toPlainText();
    if (!plaintext.isEmpty()){
        if (model == SAKDebugPage::Bin){
            QString strTemp;
            plaintext.remove(QRegExp("[^0-1]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 8 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            inputTextEdit->setText(strTemp);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKDebugPage::Oct) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-7]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            inputTextEdit->setText(strTemp);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKDebugPage::Dec) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            inputTextEdit->setText(strTemp);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKDebugPage::Hex) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9a-fA-F]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            inputTextEdit->setText(strTemp.toUpper());
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKDebugPage::Ascii) {
            plaintext.remove(QRegExp("[^\0u00-\u007f ]"));
            inputTextEdit->setText(plaintext);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKDebugPage::Local8bit) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
        }
    }
}

uint32_t SAKDebugPage::crcCalculate(QByteArray data, SAKCRCInterface::CRCModel model)
{
    int bitsWidth = crcInterface->getBitsWidth(model);
    uint8_t crc8;
    uint16_t crc16;
    uint32_t crc32;
    quint32 crc = 0;
    switch (bitsWidth) {
    case 8:
        crcInterface->crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), crc8, model);
        crc = crc8;
        break;
    case 16:
        crcInterface->crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), crc16, model);
        crc = crc16;
        break;
    case 32:
        crcInterface->crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), crc32, model);
        crc = crc32;
        break;
    default:
        break;
    }

    return crc;
}

QByteArray SAKDebugPage::cookedData(QString rawData)
{
    QByteArray data;
    TextDisplayModel textModel = inputParameters.inputModel;
    if (textModel == SAKDebugPage::Bin){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 2)));
        }
    }else if (textModel == SAKDebugPage::Oct){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 8)));
        }
    }else if (textModel == SAKDebugPage::Dec){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 10)));
        }
    }else if (textModel == SAKDebugPage::Hex){
        QStringList strList = rawData.split(' ');
        for (QString str:strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 16)));
        }
    }else if (textModel == SAKDebugPage::Ascii){
        data = rawData.toLatin1();
    }else if (textModel == SAKDebugPage::Local8bit){
        data = rawData.toLocal8Bit();
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }
    return data;
}

void SAKDebugPage::cycleTimerTimeout()
{
    on_sendPushButton_clicked();
}

void SAKDebugPage::outputMessage(QString msg, bool isInfo)
{
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    time = QString("<font color=silver>%1</font>").arg(time);
    messageTextBrowser->append(time);

    if (isInfo){
        msg = QString("<font color=blue>%1</font>").arg(msg);
    }else{
        msg = QString("<font color=red>%1</font>").arg(msg);
        QApplication::beep();
    }

    messageTextBrowser->append(msg);
    messageTextBrowser->append("");
}

void SAKDebugPage::changedDeviceStatus(bool opened)
{
    sendPushButton->setEnabled(opened);
    if (opened){

    }else{

    }
}

void SAKDebugPage::checkedBoxCycleClicked(bool checked)
{
    if (checked){

    }else{
        cycleTimer.stop();
        sendPushButton->setEnabled(true);
        cycleTimeLineEdit->setEnabled(true);
    }
}

void SAKDebugPage::cancleCycle()
{
    cycleEnableCheckBox->setChecked(false);
}

void SAKDebugPage::bytesRead(QByteArray data)
{
    /// 更新接收统计
    qlonglong receiveFrameCount = rxFramesLabel->text().toLongLong();
    receiveFrameCount += 1;
    rxFramesLabel->setText(QString::number(receiveFrameCount));
    qlonglong receiveBytesCount = rxBytesLabel->text().toLongLong();
    receiveBytesCount += data.length();
    rxBytesLabel->setText(QString::number(receiveBytesCount));

    if (showRxDataCheckBox->isChecked()){
        OutputParameters parameters = outputParameters();
        parameters.isReceivedData = true;
        emit dataReadOrwritten(data, parameters);
    }
}

void SAKDebugPage::bytesWritten(QByteArray data)
{
    qlonglong writeBytes = txBytesLabel->text().toLongLong();
    writeBytes += data.length();
    txBytesLabel->setText(QString::number(writeBytes));

    qlonglong writeFrame = txFramesLabel->text().toLongLong();
    writeFrame += 1;
    txFramesLabel->setText(QString::number(writeFrame));

    if (showTxDataCheckBox->isChecked()){
        OutputParameters parameters = outputParameters();
        parameters.isReceivedData = false;
        emit dataReadOrwritten(data, parameters);
    }
}

void SAKDebugPage::initUI()
{
    showMsCheckBox->setEnabled(true);
    showDateCheckBox->setChecked(false);
    showTimeCheckBox->setChecked(true);
    showRxDataCheckBox->setChecked(true);
    showTxDataCheckBox->setChecked(true);    
    sendPushButton->setEnabled(false);
    cycleTimeLineEdit->setValidator(new QIntValidator(10, 24*60*60*1000, cycleTimeLineEdit));

    readOutputMode();
    readInputMode();
    readCycleTime();

    rxLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(QSize(18, 18), Qt::KeepAspectRatio));
    txLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(QSize(18, 18), Qt::KeepAspectRatio));

    outputTextBroswer->setLineWrapMode(QTextEdit::WidgetWidth);

    /*
     * 设置crc参数模型
     */
    QStringList models = crcInterface->supportedParameterModels();
    crcParameterModelsComboBox->addItems(models);
}

void SAKDebugPage::registerMetaType()
{
    qRegisterMetaType<TextDisplayModel>("TextDisplayModel");
    qRegisterMetaType<OutputParameters>("OutputParameters");
}

void SAKDebugPage::outputData(QString data)
{
     outputTextBroswer->append(data);
}

void SAKDebugPage::resetSendDataCount()
{
    txBytesLabel->setText("0");
    txFramesLabel->setText("0");
}

void SAKDebugPage::resetReceiveDataCount()
{
    rxBytesLabel->setText("0");
    rxFramesLabel->setText("0");
}

void SAKDebugPage::setOutputMode(QString mode)
{
    QString option = QString("OutputMode");
    writeSetting(option, mode);
}

void SAKDebugPage::readOutputMode()
{
//    QString option = QString("OutputMode");
//    QString value = readSetting(option);

//    /// 默认使用Ascii输入
//    if (value.isEmpty()){
//        value = QString(textModel.valueToKey(Ascii));
//        setInputMode(value);
//    }

//    outputModelComboBox->setCurrentText(value);
}

void SAKDebugPage::setInputMode(QString mode)
{
    Q_UNUSED(mode);
//    QString option = QString("InputMode");
//    writeSetting(option, mode);
//    inputTextMode = mode;
}

void SAKDebugPage::readInputMode()
{
//    QString option = QString("InputMode");
//    QString value = readSetting(option);

//    /// 默认使用Ascii输入
//    if (value.isEmpty()){
//        value = QString(textModel.valueToKey(Ascii));
//    }

//    inputTextMode = value;
//    textFormatControl();

//    inputModelComboBox->setCurrentText(value);
}

void SAKDebugPage::updateTxImage()
{
//    static bool b = false;
//    if (b){
//        txLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }else{
//        txLabel->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }

//    b = !b;
}

void SAKDebugPage::updateRxImage()
{
//    static bool b = false;
//    if (b){
//        rxLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }else{
//        rxLabel->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }

//    b = !b;
}

void SAKDebugPage::setCycleTime(QString time)
{
    QString option = QString("CycleTime");
    writeSetting(option, time);
}

void SAKDebugPage::readCycleTime()
{
    QString option = QString("CycleTime");
    QString value = readSetting(option);

    cycleTimeLineEdit->setText(value);
}

QString SAKDebugPage::readSetting(QString &option)
{
    Q_UNUSED(option);
//    QSettings settings;
//    QString key = device->deviceName() + "/" + option;
//    QString value = settings.value(key).toString();

    return "  ";
}

void SAKDebugPage::writeSetting(QString &option, QString &value)
{
    Q_UNUSED(option);
    Q_UNUSED(value);
//    QSettings settings;
//    QString key = device->deviceName() + "/" + option;

//    settings.setValue(key, value);
}

void SAKDebugPage::initUiPointer()
{
    /*
     * 设备管理组
     */
    switchPushButton        = ui->switchPushButton;
    refreshPushButton       = ui->refreshPushButton;
    deviceSettingGroupBox   = ui->deviceSettingGroupBox;

    /*
     * 消息输出组
     */
    messageTextBrowser      = ui->messageTextBrowser;

    /*
     * 输入设置组
     */
    inputModelComboBox      = ui->inputModelComboBox;
    cycleEnableCheckBox     = ui->cycleEnableCheckBox;
    cycleTimeLineEdit       = ui->cycleTimeLineEdit;
    readinFilePushButton    = ui->readinFilePushButton;
    addCRCCheckBox          = ui->addCRCCheckBox;
    bigeEndianCheckBox      = ui->bigeEndianCheckBox;
    clearInputPushButton    = ui->clearInputPushButton;
    sendPushButton          = ui->sendPushButton;
    inputTextEdit           = ui->inputTextEdit;
    crcParameterModelsComboBox = ui->crcParameterModelsComboBox;
    crcLabel                = ui->crcLabel;

    /*
     * 输出设置组
     */
    rxLabel                 = ui->rxLabel;
    txLabel                 = ui->txLabel;
    outputModelComboBox     = ui->outputModelComboBox;
    autoWrapCheckBox        = ui->autoWrapCheckBox;
    showDateCheckBox        = ui->showDateCheckBox;
    showTimeCheckBox        = ui->showTimeCheckBox;
    showMsCheckBox          = ui->showMsCheckBox;
    showRxDataCheckBox      = ui->showRxDataCheckBox;
    showTxDataCheckBox      = ui->showTxDataCheckBox;
    clearOutputPushButton   = ui->clearOutputPushButton;
    saveOutputPushButton    = ui->saveOutputPushButton;
    outputTextBroswer       = ui->outputTextBroswer;

    /*
     * 数据管理组
     */
    rxFramesLabel           = ui->rxFramesLabel;
    txFramesLabel           = ui->txFramesLabel;
    rxBytesLabel            = ui->rxBytesLabel;
    txBytesLabel            = ui->txBytesLabel;
    resetRxCountPushButton  = ui->resetRxCountPushButton;
    resetTxCountPushButton  = ui->resetTxCountPushButton;

    /*
     * 其他设计组
     */
    autoResponseSettingPushButton  = ui->autoResponseSettingPushButton;
    highlightSettingPushButton     = ui->highlightSettingPushButton;
    readWriteSettingPushButton     = ui->readWriteSettingPushButton;
}

void SAKDebugPage::on_autoResponseSettingPushButton_clicked()
{
    SAKAutoResponseSettingPanel *window = new SAKAutoResponseSettingPanel(this);
    window->show();
}

void SAKDebugPage::on_highlightSettingPushButton_clicked()
{
    SAKHighlighterSettingPanel *window = new SAKHighlighterSettingPanel(outputTextBroswer->document(), this);
    window->show();
}

void SAKDebugPage::on_readWriteSettingPushButton()
{
    SAKReadWriteSetting *window = new SAKReadWriteSetting(this);
    window->show();
}

void SAKDebugPage::on_resetRxCountPushButton_clicked()
{
    receiveFrames = 0;
    setLabelText(rxFramesLabel, receiveFrames);

    receiveBytes = 0;
    setLabelText(rxBytesLabel, receiveBytes);
}

void SAKDebugPage::on_resetTxCountPushButton_clicked()
{
    sendFrames = 0;
    setLabelText(txFramesLabel, sendFrames);

    sendBytes = 0;
    setLabelText(txFramesLabel, sendBytes);
}

void SAKDebugPage::on_outputModelComboBox_currentTextChanged(const QString &text)
{
    // 在ui初始化的时候，会出现text为empty的情况
    if (text.isEmpty()){
        return;
    }

    QMetaEnum model = QMetaEnum::fromType<TextDisplayModel>();
    bool ok = false;
    int ret = model.keyToValue(text.toLatin1().data(), &ok);
    if (ok){
        outputTextModel = static_cast<TextDisplayModel>(ret);
    }else{
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Output text model error!";
    }
}

void SAKDebugPage::on_autoWrapCheckBox_clicked()
{

}

void SAKDebugPage::on_clearOutputPushButton_clicked()
{
    outputTextBroswer->clear();
}

void SAKDebugPage::on_saveOutputPushButton_clicked()
{
    QString outFileName = QFileDialog::getSaveFileName();
    QFile outFile(outFileName);

    if(outFile.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream outStream(&outFile);
        outStream << outputTextBroswer->toPlainText();
        outFile.flush();
        outFile.close();
    }else{
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Can not open file:" << outFile.fileName() << "," << outFile.errorString();
    }
}

void SAKDebugPage::on_inputModelComboBox_currentTextChanged(const QString &text)
{
    /*
     *  在ui初始化的时候，会出现text为empty的情况
     */
    if (text.isEmpty()){
        return;
    }

    QMetaEnum model = QMetaEnum::fromType<TextDisplayModel>();
    bool ok = false;
    int ret = model.keyToValue(text.toLatin1().data(), &ok);
    if (ok){
        inputParameters.inputModel = static_cast<TextDisplayModel>(ret);
    }else{
        inputParameters.inputModel = TextDisplayModel::Ascii;
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Input text model error!";
    }

    /*
     * 处理输入框已存在的数据
     */
    inputTextEdit->blockSignals(true);
    formattingInputText(inputParameters.inputModel);
    inputTextEdit->blockSignals(false);
}

void SAKDebugPage::on_cycleEnableCheckBox_clicked()
{
    if (cycleEnableCheckBox->isChecked()){
        inputParameters.sendCircularly = true;
        if (!cycleTimer.isActive()){
            cycleTimer.start();
        }
    }else{
        inputParameters.sendCircularly = false;
        if (cycleTimer.isActive()){
            cycleTimer.stop();
        }
    }
}

void SAKDebugPage::on_cycleTimeLineEdit_textChanged(const QString &text)
{
    if (text.isEmpty()){
        outputMessage(tr("循环时间不能为空"), false);
        return;
    }

    bool ok = 0;
    int ret = text.toInt(&ok);
    if (ok){
        inputParameters.cycleTime = static_cast<quint32>(ret);
    }else{
        inputParameters.cycleTime = 100;
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Cycle time setting error! Cycle time will be set as 100ms!";
    }
}

void SAKDebugPage::on_readinFilePushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly)){
            QByteArray data = file.readAll();
            inputTextEdit->setText(QString(data).toUtf8());
            file.close();
        }else{
            qWarning() << QString("%1 %2").arg(tr("无法打开文件")).arg(fileName);
        }
    }
}

void SAKDebugPage::on_addCRCCheckBox_clicked()
{
    inputParameters.addCRC = addCRCCheckBox->isChecked();
}

void SAKDebugPage::on_bigeEndianCheckBox_clicked()
{
    inputParameters.bigEnfian = bigeEndianCheckBox->isChecked();
}

void SAKDebugPage::on_clearInputPushButton_clicked()
{
    inputTextEdit->clear();
}

void SAKDebugPage::on_sendPushButton_clicked()
{
    QString data = inputTextEdit->toPlainText();
    if (data.isEmpty()){
        outputMessage(tr("请输入发送数据后尝试"), false);
        return;
    }
#if 0
    QMetaEnum textModelTemp = QMetaEnum::fromType<TextDisplayModel>();
    bool ok = false;
    TextDisplayModel model = static_cast<TextDisplayModel>(textModelTemp.keyToValue(inputModelComboBox->currentText().toLatin1().data(), &ok));
    if (!ok){
        model = SAKTabPage::Hex;
    }
#endif
    emit sendRawData(data, inputParameters);
}

void SAKDebugPage::on_inputTextEdit_textChanged()
{
    inputTextEdit->blockSignals(true);
    formattingInputText(inputParameters.inputModel);
    inputTextEdit->blockSignals(false);

    /*
     * 计算crc,并显示
     */
    QString rawData = inputTextEdit->toPlainText();
    QByteArray data = cookedData(inputTextEdit->toPlainText());
    uint32_t crc = crcCalculate(data, inputParameters.crcModel);
    int bitsWidth = crcInterface->getBitsWidth(inputParameters.crcModel);
    crcLabel->setText(QString("0x") + QString("%1").arg(QString::number(crc, 16), bitsWidth/8*2, '0').toUpper());
}

void SAKDebugPage::on_crcParameterModelsComboBox_currentTextChanged(const QString &text)
{
    QMetaEnum model = QMetaEnum::fromType<SAKCRCInterface::CRCModel>();
    bool ok;
    int modelValue = model.keyToValue(text.toLatin1().constData(), &ok);
    if (ok){
        inputParameters.crcModel = static_cast<SAKCRCInterface::CRCModel>(modelValue);
    }else{
        inputParameters.crcModel = SAKCRCInterface::CRC_8;
    }

    /*
     * 更新crc值
     */
    on_inputTextEdit_textChanged();
}
