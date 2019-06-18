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

#include "SAKTabPage.hh"
#include "ui_SAKTabPage.h"
#include "SAKDataFactory.hh"
#include "SAKCRCInterface.hh"
#include "SAKHighlighterSettingPanel.hh"

SAKTabPage::SAKTabPage(QWidget *parent)
    :QWidget(parent)
    ,crcInterface (new SAKCRCInterface(this))
    ,inputModel (Local8bit)    
    ,ui (new Ui::SAKTabPage)
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
    dataFactory = new SAKDataFactory;
    connect(this, &SAKTabPage::sendRawData, dataFactory, &SAKDataFactory::handleTheDataThatNeedsToBeSent);
    connect(this, &SAKTabPage::dataReadOrwritten, dataFactory, &SAKDataFactory::handleTheDataThatNeedsToBeOutputted);
    connect(dataFactory, &SAKDataFactory::sendBytes, this, &SAKTabPage::writeBytes);
    connect(dataFactory, &SAKDataFactory::outputData, this, &SAKTabPage::outputData);
    dataFactory->start();

    /*
     * 初始化输入输出模式
     */
    inputModelComboBox->clear();
    outputModelComboBox->clear();
    QMetaEnum textModel = QMetaEnum::fromType<SAKTabPage::TextDisplayModel>();
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
    connect(this, &SAKTabPage::deviceStatusChanged, this, &SAKTabPage::changedDeviceStatus);
}

SAKTabPage::~SAKTabPage()
{
    delete ui;
    delete logCategory;
}

unsigned long SAKTabPage::readDelayTimes()
{
    return 0;
}

unsigned long SAKTabPage::writeDelayTimes()
{
    return 0;
}

void SAKTabPage::setUpController()
{
    QWidget *controller = controllerWidget();
    if (controller){
        QGridLayout *layout = reinterpret_cast<QGridLayout *>(deviceSettingGroupBox->layout());
        layout->addWidget(controller, 2, 0, 1, 2);
    }
}

SAKTabPage::OutputParameters SAKTabPage::outputParameters()
{
    OutputParameters parameters;
    parameters.showMS = showMsCheckBox->isChecked();
    parameters.showDate = showDateCheckBox->isChecked();
    parameters.showTime = showTimeCheckBox->isChecked();

    QMetaEnum outputTextModelTemp = QMetaEnum::fromType<TextDisplayModel>();
    bool ok = false;
    parameters.textModel = static_cast<TextDisplayModel>(outputTextModelTemp.keyToValue(outputModelComboBox->currentText().toLatin1().data(), &ok));
    if (!ok){
        parameters.textModel = SAKTabPage::Hex;
    }

    return parameters;
}

void SAKTabPage::setLabelText(QLabel *label, quint64 text)
{
    QString str = QString::number(text);
    label->setText(str);
}

void SAKTabPage::formattingInputText(TextDisplayModel model)
{
    QString plaintext = inputTextEdit->toPlainText();
    if (!plaintext.isEmpty()){
        if (model == SAKTabPage::Bin){
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
        }else if(model == SAKTabPage::Oct) {
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
        }else if(model == SAKTabPage::Dec) {
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
        }else if(model == SAKTabPage::Hex) {
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
        }else if(model == SAKTabPage::Ascii) {
            plaintext.remove(QRegExp("[^\0u00-\u007f ]"));
            inputTextEdit->setText(plaintext);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKTabPage::Local8bit) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
        }
    }
}

void SAKTabPage::outputMessage(QString msg, bool isInfo)
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

void SAKTabPage::changedDeviceStatus(bool opened)
{
    sendPushButton->setEnabled(opened);
    if (opened){

    }else{

    }
}

void SAKTabPage::checkedBoxCycleClicked(bool checked)
{
    if (checked){

    }else{
        cycleTimer.stop();
        sendPushButton->setEnabled(true);
        cycleTimeLineEdit->setEnabled(true);
    }
}

void SAKTabPage::cycleTimerTimeout()
{
//    if (device->isOpen()){
//        emit need2writeBytes(dataBytes());
//    }else {
//        cycleTimer->stop();
//        outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
//    }
}

void SAKTabPage::cancleCycle()
{
    cycleEnableCheckBox->setChecked(false);
}

void SAKTabPage::bytesRead(QByteArray data)
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

void SAKTabPage::bytesWritten(QByteArray data)
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

void SAKTabPage::initUI()
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

void SAKTabPage::registerMetaType()
{
    qRegisterMetaType<TextDisplayModel>("TextDisplayModel");
    qRegisterMetaType<OutputParameters>("OutputParameters");
}

void SAKTabPage::outputData(QString data)
{
     outputTextBroswer->append(data);
}

void SAKTabPage::resetSendDataCount()
{
    txBytesLabel->setText("0");
    txFramesLabel->setText("0");
}

void SAKTabPage::resetReceiveDataCount()
{
    rxBytesLabel->setText("0");
    rxFramesLabel->setText("0");
}

void SAKTabPage::setOutputMode(QString mode)
{
    QString option = QString("OutputMode");
    writeSetting(option, mode);
}

void SAKTabPage::readOutputMode()
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

void SAKTabPage::setInputMode(QString mode)
{
    Q_UNUSED(mode);
//    QString option = QString("InputMode");
//    writeSetting(option, mode);
//    inputTextMode = mode;
}

void SAKTabPage::readInputMode()
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

void SAKTabPage::updateTxImage()
{
//    static bool b = false;
//    if (b){
//        txLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }else{
//        txLabel->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }

//    b = !b;
}

void SAKTabPage::updateRxImage()
{
//    static bool b = false;
//    if (b){
//        rxLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }else{
//        rxLabel->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    }

//    b = !b;
}

void SAKTabPage::setCycleTime(QString time)
{
    QString option = QString("CycleTime");
    writeSetting(option, time);
}

void SAKTabPage::readCycleTime()
{
    QString option = QString("CycleTime");
    QString value = readSetting(option);

    cycleTimeLineEdit->setText(value);
}

QString SAKTabPage::readSetting(QString &option)
{
    Q_UNUSED(option);
//    QSettings settings;
//    QString key = device->deviceName() + "/" + option;
//    QString value = settings.value(key).toString();

    return "  ";
}

void SAKTabPage::writeSetting(QString &option, QString &value)
{
    Q_UNUSED(option);
    Q_UNUSED(value);
//    QSettings settings;
//    QString key = device->deviceName() + "/" + option;

//    settings.setValue(key, value);
}

void SAKTabPage::initUiPointer()
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
    clearInputPushButton    = ui->clearInputPushButton;
    sendPushButton          = ui->sendPushButton;
    inputTextEdit           = ui->inputTextEdit;
    crcParameterModelsComboBox = ui->crcParameterModelsComboBox;

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

void SAKTabPage::on_autoResponseSettingPushButton_clicked()
{
    SAKAutoResponseSettingPanel *window = new SAKAutoResponseSettingPanel(this);
    window->show();
}

void SAKTabPage::on_highlightSettingPushButton_clicked()
{
    SAKHighlighterSettingPanel *window = new SAKHighlighterSettingPanel(outputTextBroswer->document(), this);
    window->show();
}

void SAKTabPage::on_readWriteSettingPushButton()
{
    SAKReadWriteSetting *window = new SAKReadWriteSetting(this);
    window->show();
}

void SAKTabPage::on_resetRxCountPushButton_clicked()
{
    receiveFrames = 0;
    setLabelText(rxFramesLabel, receiveFrames);

    receiveBytes = 0;
    setLabelText(rxBytesLabel, receiveBytes);
}

void SAKTabPage::on_resetTxCountPushButton_clicked()
{
    sendFrames = 0;
    setLabelText(txFramesLabel, sendFrames);

    sendBytes = 0;
    setLabelText(txFramesLabel, sendBytes);
}

void SAKTabPage::on_outputModelComboBox_currentTextChanged(const QString &text)
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

void SAKTabPage::on_autoWrapCheckBox_clicked()
{

}

void SAKTabPage::on_clearOutputPushButton_clicked()
{
    outputTextBroswer->clear();
}

void SAKTabPage::on_saveOutputPushButton_clicked()
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

void SAKTabPage::on_inputModelComboBox_currentTextChanged(const QString &text)
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
        inputModel = static_cast<TextDisplayModel>(ret);
    }else{
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Input text model error!";
    }

    /*
     * 处理输入框已存在的数据
     */
    inputTextEdit->blockSignals(true);
    formattingInputText(inputModel);
    inputTextEdit->blockSignals(false);
}

void SAKTabPage::on_cycleEnableCheckBox_clicked()
{
    if (cycleEnableCheckBox->isChecked()){
        cyclEnable = true;
    }else{
        cyclEnable = false;
    }
}

void SAKTabPage::on_cycleTimeLineEdit_textChanged(const QString &text)
{
    if (text.isEmpty()){
        return;
    }

    bool ok = 0;
    int ret = text.toInt(&ok);
    if (ok){
        cycleTime = static_cast<quint32>(ret);
    }else{
        cycleTime = 100;
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Cycle time setting error! Cycle time will be set as 100ms!";
    }
}

void SAKTabPage::on_readinFilePushButton_clicked()
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

void SAKTabPage::on_clearInputPushButton_clicked()
{
    inputTextEdit->clear();
}

void SAKTabPage::on_sendPushButton_clicked()
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
    emit sendRawData(data, inputModel);
}

void SAKTabPage::on_inputTextEdit_textChanged()
{
    inputTextEdit->blockSignals(true);
    formattingInputText(inputModel);
    inputTextEdit->blockSignals(false);
}

void SAKTabPage::on_crcParameterModelsComboBox_currentTextChanged(const QString &text)
{
    qDebug() << __FUNCTION__ << text;
}
