/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#ifdef _MSC_VER
#pragma execution_character_set("utf-8")
#endif

#include <QDateTime>
#include <QIntValidator>
#include <QScrollBar>
#include <QFileDialog>
#include <QTextStream>
#include <QPixmap>
#include <QSettings>
#include <QKeyEvent>
#include <QDebug>

#include "SAKIODeviceWidget2.hh"
#include "SAKHighlighterSettingPanel.hpp"
#include "SAKAutoResponseSettingPanel.hpp"
#include "SAKReadWriteSetting.hh"

#include "ui_SAKIODeviceWidget2.h"

SAKIODeviceWidget2::SAKIODeviceWidget2(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKIODeviceWidget2)
    ,device(_device)
    ,controler(_controler)
    ,cycleTimer(new QTimer)
    ,customControlerLayout(new QHBoxLayout)
    ,delayTimer(new QTimer)
    ,clearInfoTimer(new QTimer)
{
    ui->setupUi(this);    

    initUiPointer();
    initIODevice();

    inputTextModelComboBox = ui->comboBoxInputMode;
    outputTextModelComboBox = ui->comboBoxOutputMode;

    rwParameterSettingButton = ui->pushButtonRWParameter;


    /// 初始化输入输出模式
    inputTextModelComboBox->clear();
    outputTextModelComboBox->clear();
    textModel = QMetaEnum::fromType<SAKIODeviceWidget2::TextDisplayModel>();
    for (int i = 0; i < textModel.keyCount(); i++){
        inputTextModelComboBox->addItem(QString(textModel.valueToKey(i)));
        outputTextModelComboBox->addItem(QString(textModel.valueToKey(i)));
    }

    Connect();
    setCustomControler(controler);
    initUI();

    clearInfoTimer->setInterval(3*1000);
    connect(clearInfoTimer, SIGNAL(timeout()), this, SLOT(clearInfo()));
}

SAKIODeviceWidget2::~SAKIODeviceWidget2()
{
    delete ui;
}

void SAKIODeviceWidget2::initUI()
{
    ui->checkBoxOutputMS->setEnabled(true);
    ui->checkBoxOutputDate->setChecked(false);
    ui->checkBoxOutputTime->setChecked(true);
    ui->lineEditCycleTime->setValidator(new QIntValidator(10, 24*60*60*1000, ui->lineEditCycleTime));
    ui->lineEditBytesDelayTime->setValidator(new QIntValidator(10, 24*60*60*1000, ui->lineEditBytesDelayTime));

#if 0   /// 按钮图标
    ui->pushButtonClearInput->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    ui->pushButtonClearOutput->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    ui->pushButtonReadInFile->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->pushButtonSaveOutput->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    ui->pushButtonRefresh->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
#endif

    readOutputMode();
    readInputMode();
    readCycleTime();
    readDelayTime();
    readAutoResponseFlag();

    ui->labelRX->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    ui->labelTX->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    ui->labelInfoIcon->setPixmap(QPixmap(":/images/Info.png").scaled(QSize(ui->labelInfoIcon->width()-4,
                                                                           ui->labelInfoIcon->height() - 4),
                                                                     Qt::KeepAspectRatio));

    ui->textBrowserOutputData->setLineWrapMode(QTextEdit::WidgetWidth);
}

void SAKIODeviceWidget2::outputTimeInfoCheckBoxClicked(bool checked)
{
    if (checked){
        ui->checkBoxOutputMS->setEnabled(true);
    }else {
        ui->checkBoxOutputMS->setChecked(false);
        ui->checkBoxOutputMS->setEnabled(false);
    }
}

void SAKIODeviceWidget2::outputReceiveDataOnlyCheckBoxClicked(bool cheaked)
{
    ui->textBrowserOutputData->clear();

    if (cheaked){
        ui->checkBoxOutputDate->setChecked(false);
        ui->checkBoxOutputDate->setEnabled(false);
        ui->checkBoxOutputTime->setChecked(false);
        ui->checkBoxOutputTime->setEnabled(false);
        ui->checkBoxOutputMS->setChecked(false);
        ui->checkBoxOutputMS->setEnabled(false);
    }else{
        ui->checkBoxOutputDate->setChecked(false);
        ui->checkBoxOutputDate->setEnabled(true);
        ui->checkBoxOutputTime->setChecked(true);
        ui->checkBoxOutputTime->setEnabled(true);
        ui->checkBoxOutputMS->setChecked(false);
        ui->checkBoxOutputMS->setEnabled(true);
    }
}

void SAKIODeviceWidget2::openOrClose()
{
    if (device->isOpen()){
        close();
    }else{
        open();
    }
}

void SAKIODeviceWidget2::setCustomControler(SAKIODeviceControler *controler)
{
    customControlerLayout->addWidget(controler);
    customControlerLayout->setMargin(0);
    ui->groupBoxCustom->setLayout(customControlerLayout);
}

void SAKIODeviceWidget2::setRefreshPBtText(QString text)
{
    ui->pushButtonRefresh->setText(text);
}

void SAKIODeviceWidget2::Connect()
{
    /// 打开/关闭设备
    connect(ui->pushButtonOpen, SIGNAL(clicked(bool)), this, SLOT(openOrClose()));
    connect(this, SIGNAL(need2open()), controler, SLOT(open()));
    connect(this, SIGNAL(need2close()), device, SLOT(close()));
    connect(controler, SIGNAL(need2open(QString,QString)), device, SLOT(open(QString,QString)));
    connect(controler, SIGNAL(need2open(QString,QString,QString,QString)), device, SLOT(open(QString,QString,QString,QString)));
    connect(controler, SIGNAL(need2open(QString,QString,QString,QString,QString)), device, SLOT(open(QString,QString,QString,QString,QString)));
    connect(device, SIGNAL(deviceOpenSuccessfully()), this, SLOT(afterDeviceOpen()));
    connect(device, SIGNAL(deviceCloseSuccessfully()), this, SLOT(afterDeviceClose()));

    /// 刷新设备
    connect(ui->pushButtonRefresh, SIGNAL(clicked(bool)), controler, SLOT(refresh()));

    connect(cycleTimer, SIGNAL(timeout()), this, SLOT(cycleTimerTimeout()));
    connect(delayTimer, SIGNAL(timeout()), this, SLOT(delayTimerTimeout()));
    connect(ui->checkBoxCycle, SIGNAL(clicked(bool)), this, SLOT(checkedBoxCycleClicked(bool)));
    connect(ui->checkBoxBytesDelay, SIGNAL(clicked(bool)), this, SLOT(checkedBoxDelayClicked(bool)));
    connect(device, SIGNAL(errorStr(QString)), this, SLOT(outputErrorString(QString)));
    connect(device, SIGNAL(infoStr(QString)), this, SLOT(outputInformationString(QString)));
    connect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
    connect(ui->pushButtonReadInFile, SIGNAL(clicked(bool)), this, SLOT(openFile()));
    connect(ui->pushButtonSaveOutput, SIGNAL(clicked(bool)), this, SLOT(saveOutputData()));

    connect(ui->checkBoxOutputTime, SIGNAL(clicked(bool)), this, SLOT(outputTimeInfoCheckBoxClicked(bool)));
    connect(ui->checkBoxOutputReceiveDataOnly, SIGNAL(clicked(bool)), this, SLOT(outputReceiveDataOnlyCheckBoxClicked(bool)));

    /// 数据收发
    connect(ui->pushButtonSendData, SIGNAL(clicked(bool)), this, SLOT(writeBytes()));
    connect(this, SIGNAL(need2writeBytes(QByteArray)), device, SLOT(writeBytes(QByteArray)));
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(bytesRead(QByteArray)));
    connect(device, SIGNAL(bytesWritten(QByteArray)), this, SLOT(bytesWritten(QByteArray)));

    /// 重置收发计数
    connect(ui->pushButtonResetReceiveDataCount, SIGNAL(clicked(bool)), this, SLOT(resetReceiveDataCount()));
    connect(ui->pushButtonResetSendDataCount, SIGNAL(clicked(bool)), this, SLOT(resetSendDataCount()));

    /// 循环周期
    connect(ui->lineEditCycleTime, SIGNAL(textChanged(QString)), this, SLOT(setCycleTime(QString)));
    /// 字节间延时
    connect(ui->lineEditBytesDelayTime, SIGNAL(textChanged(QString)), this, SLOT(setDelayTime(QString)));

    /// 指示灯
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(updateRxImage()));
    connect(device, SIGNAL(bytesWritten(QByteArray)),  this, SLOT(updateTxImage()));

    /// 文本模式
    connect(ui->comboBoxInputMode, SIGNAL(currentTextChanged(QString)), this , SLOT(setInputMode(QString)));
    connect(ui->comboBoxOutputMode, SIGNAL(currentTextChanged(QString)), this, SLOT(setOutputMode(QString)));

    /// 循环发送与字节间延时发送
    connect(ui->checkBoxBytesDelay, SIGNAL(clicked(bool)), this, SLOT(cancleCycle()));
    connect(ui->checkBoxCycle, SIGNAL(clicked(bool)), this, SLOT(cancleBytesDelay()));

    /// 自动回复
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(handleReadBytes(QByteArray)));
}

void SAKIODeviceWidget2::afterDeviceOpen()
{
    controler->afterDeviceOpen();
    ui->pushButtonRefresh->setEnabled(false);
    ui->pushButtonOpen->setText(tr("关闭"));
}

void SAKIODeviceWidget2::afterDeviceClose()
{
    controler->afterDeviceClose();
    ui->pushButtonRefresh->setEnabled(true);
    ui->pushButtonOpen->setText(tr("打开"));

    if (cycleTimer->isActive()){
        cycleTimer->stop();
        ui->pushButtonSendData->setEnabled(true);
        ui->lineEditCycleTime->setEnabled(true);
    }

    if (delayTimer->isActive()){
        delayTimer->stop();
        ui->pushButtonSendData->setEnabled(true);
        ui->lineEditCycleTime->setEnabled(true);
    }
}

void SAKIODeviceWidget2::outputInfo(QString info, QString color, bool prefix)
{
    QString strTemp;
    if (prefix){
        strTemp += QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        strTemp = QString("<font color=silver>%1 </font>").arg(strTemp);
    }

    strTemp += QString("<font color=%1>%2</font>").arg(color).arg(info);
    ui->labelOutput->setText(strTemp);
    clearInfoTimer->start();
}

void SAKIODeviceWidget2::outputErrorString(QString str)
{
    outputInfo(str, "red");
}

void SAKIODeviceWidget2::outputInformationString(QString str)
{
    outputInfo(str, "green");
}

void SAKIODeviceWidget2::clearInfo()
{
    ui->labelOutput->clear();
    if (clearInfoTimer->isActive()){
        clearInfoTimer->stop();
    }
}

void SAKIODeviceWidget2::writeBytes()
{
    QString tipStr = tr("设备未就绪，本次发送操作取消！");

    if (ui->checkBoxCycle->isChecked()){
        if (device->isOpen()){
            qlonglong cycleTime = ui->lineEditCycleTime->text().toLongLong();
            if (cycleTime == 0){
                cycleTime = 1000;
            }
            cycleTimer->start(static_cast<int>(cycleTime));
            ui->pushButtonSendData->setEnabled(false);
            ui->lineEditCycleTime->setEnabled(false);
        }else {
             outputInfo(tipStr, "red");
        }
    }else if(ui->checkBoxBytesDelay->isChecked()){
        if (device->isOpen()){
            dataTemp = dataBytes();
            if (dataTemp.isEmpty()){
                dataTemp = QByteArray("(null)");
            }

            /// 马上发送第一个字节，如果本身只有一个字节，停止后续操作。
            delayTimerTimeout();
            if (!dataTemp.isEmpty()){
                qlonglong delayTime = ui->lineEditBytesDelayTime->text().toLongLong();
                if (delayTime == 0){
                    delayTime = 1000;
                }
                delayTimer->start(static_cast<int>(delayTime));
                ui->pushButtonSendData->setEnabled(false);
                ui->lineEditCycleTime->setEnabled(false);
            }
        }else {
             outputInfo(tipStr, "red");
        }
    }else{
        if (device->isOpen()){
            emit need2writeBytes(dataBytes());
        }else {
            outputInfo(tipStr, "red");
        }
    }
}

void SAKIODeviceWidget2::checkedBoxCycleClicked(bool checked)
{
    if (checked){

    }else{
        cycleTimer->stop();
        ui->pushButtonSendData->setEnabled(true);
        ui->lineEditCycleTime->setEnabled(true);
    }
}

void SAKIODeviceWidget2::checkedBoxDelayClicked(bool checked)
{
    if (checked){

    }else {
        delayTimer->stop();
        ui->pushButtonSendData->setEnabled(true);
        ui->lineEditCycleTime->setEnabled(true);
    }
}

void SAKIODeviceWidget2::cycleTimerTimeout()
{
    if (device->isOpen()){
        emit need2writeBytes(dataBytes());
    }else {
        cycleTimer->stop();
        outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
    }
}

void SAKIODeviceWidget2::delayTimerTimeout()
{
    if (device->isOpen()){
        if (dataTemp.isEmpty()){
            delayTimer->stop();
            ui->pushButtonSendData->setEnabled(true);
            ui->lineEditCycleTime->setEnabled(true);
        }else {
            QByteArray array;
            array[0] = dataTemp.at(0);
            emit need2writeBytes(array);
            dataTemp.remove(0, 1);
            if (dataTemp.isEmpty()){
                delayTimer->stop();
                ui->pushButtonSendData->setEnabled(true);
                ui->lineEditCycleTime->setEnabled(true);
            }
        }
    }else {
        delayTimer->stop();
        outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
    }
}

void SAKIODeviceWidget2::cancleBytesDelay()
{
    ui->checkBoxBytesDelay->setChecked(false);
}

void SAKIODeviceWidget2::cancleCycle()
{
    ui->checkBoxCycle->setChecked(false);
}

QByteArray SAKIODeviceWidget2::dataBytes()
{
    QByteArray data;

    QString str = ui->textEditInputData->toPlainText();
    if (str.isEmpty()){
        str = "(null)";
        data = str.toLatin1();
        return data;
    }

    if (inputTextMode.compare(QString(textModel.valueToKey(Bin))) == 0){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 2)));
        }
    }else if (inputTextMode.compare(QString(textModel.valueToKey(Oct))) == 0){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 8)));
        }
    }else if (inputTextMode.compare(QString(textModel.valueToKey(Dec))) == 0){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 10)));
        }
    }else if (inputTextMode.compare(QString(textModel.valueToKey(Hex))) == 0){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 16)));
        }
    }else if (inputTextMode.compare(QString(textModel.valueToKey(Ascii))) == 0){
        data = ui->textEditInputData->toPlainText().toLatin1();
    }else if (inputTextMode.compare(QString(textModel.valueToKey(Local8bit))) == 0){
        data = ui->textEditInputData->toPlainText().toLocal8Bit();
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }

    return data;
}

void SAKIODeviceWidget2::textFormatControl()
{
    disconnect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
    QString plaintext = ui->textEditInputData->toPlainText();
    if (plaintext.isEmpty()){
        connect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
        return;
    }else {
        if (inputTextMode.compare(QString(textModel.valueToKey(Bin))) == 0){
            QString strTemp;
            plaintext.remove(QRegExp("[^0-1]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 8 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEditInputData->setText(strTemp);
            ui->textEditInputData->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Oct))) == 0) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-7]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEditInputData->setText(strTemp);
            ui->textEditInputData->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Dec))) == 0) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEditInputData->setText(strTemp);
            ui->textEditInputData->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Hex))) == 0) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9a-fA-F]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            ui->textEditInputData->setText(strTemp.toUpper());
            ui->textEditInputData->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Ascii))) == 0) {
            plaintext.remove(QRegExp("[^\0u00-\u007f ]"));
            ui->textEditInputData->setText(plaintext);
            ui->textEditInputData->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Local8bit))) == 0) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
        }
    }
    connect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
}

void SAKIODeviceWidget2::bytesRead(QByteArray data)
{
    /// 更新接收统计
    qlonglong receiveFrameCount = ui->labelReceiveFrames->text().toLongLong();
    receiveFrameCount += 1;
    ui->labelReceiveFrames->setText(QString::number(receiveFrameCount));
    qlonglong receiveBytesCount = ui->labelReceiveBytes->text().toLongLong();
    receiveBytesCount += data.length();
    ui->labelReceiveBytes->setText(QString::number(receiveBytesCount));

    refreshOutputData(data, true);
}

void SAKIODeviceWidget2::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("打开文件"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly)){
            QByteArray data = file.readAll();
            ui->textEditInputData->setText(QString(data).toUtf8());
            file.close();
        }else{
            qWarning() << QString("%1 %2").arg(tr("无法打开文件")).arg(fileName);
        }
    }
}

void SAKIODeviceWidget2::saveOutputData()
{
    QString outFileName = QFileDialog::getSaveFileName();
    QFile outFile(outFileName);

    outFile.open(QIODevice::WriteOnly|QIODevice::Text);

    QTextStream outStream(&outFile);
    outStream << ui->textBrowserOutputData->toPlainText();

    outFile.flush();
    outFile.close();
}

void SAKIODeviceWidget2::bytesWritten(QByteArray data)
{
    qlonglong writeBytes = ui->labelSendBytes->text().toLongLong();
    writeBytes += data.length();
    ui->labelSendBytes->setText(QString::number(writeBytes));

    qlonglong writeFrame = ui->labelSendFrames->text().toLongLong();
    writeFrame += 1;
    ui->labelSendFrames->setText(QString::number(writeFrame));

    refreshOutputData(data, false);
}

void SAKIODeviceWidget2::refreshOutputData(QByteArray &data, bool isReceivedData)
{
    if ((!showRxDataCheckBox->isChecked()) && isReceivedData){
        return;
    }

    if ((!showTxDataCheckBox->isChecked()) && (!isReceivedData)){
        return;
    }

    QString str;
    if (!ui->checkBoxOutputReceiveDataOnly->isChecked()){
        str.append("[");
    }

    if (ui->checkBoxOutputDate->isChecked()){
        str.append(QDate::currentDate().toString("yyyy/MM/dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (ui->checkBoxOutputTime->isChecked()){
        if (ui->checkBoxOutputMS->isChecked()){
            str.append(QTime::currentTime().toString("hh:mm:ss.z "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (!ui->checkBoxOutputReceiveDataOnly->isChecked()){
        if (isReceivedData){
            str.append("<font color=blue>Rx</font>");
        }else {
            str.append("<font color=purple>Tx</font>");
        }
        str.append("<font color=silver>] </font>");
    }

    outputTextMode = ui->comboBoxOutputMode->currentText();
    if (outputTextMode.compare(QString(textModel.valueToKey(Bin))) == 0){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 2), 8, '0'));
        }
    }else if (outputTextMode.compare(QString(textModel.valueToKey(Oct))) == 0){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 8), 3, '0'));
        }
    }else if (outputTextMode.compare(QString(textModel.valueToKey(Dec))) == 0){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 10)));
        }
    }else if (outputTextMode.compare(QString(textModel.valueToKey(Hex))) == 0){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 16), 2, '0'));
        }
    }else if (outputTextMode.compare(QString(textModel.valueToKey(Ascii))) == 0){
        str.append(QString(data));
    }else if (outputTextMode.compare(QString(textModel.valueToKey(Local8bit))) == 0){
        str.append(QString::fromLocal8Bit(data));
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
    }

    if (ui->checkBoxOutputReceiveDataOnly->isChecked()){
        if (!isReceivedData){
            return;
        }

        /**
         * 以下这种追加文本的方式存在问题，在文本较多，追加频繁时，界面卡顿。
         **/
        if (!ui->textBrowserOutputData->toPlainText().isEmpty()){
            str = ui->textBrowserOutputData->toPlainText() + " " + str;
        }
        ui->textBrowserOutputData->setText(QString("<font color=blue>%1</font>").arg(str));
        ui->textBrowserOutputData->verticalScrollBar()->setSliderPosition(ui->textBrowserOutputData->verticalScrollBar()->maximum());
    }else {
        ui->textBrowserOutputData->append(str);
    }
}

void SAKIODeviceWidget2::resetSendDataCount()
{
    ui->labelSendBytes->setText("0");
    ui->labelSendFrames->setText("0");
}

void SAKIODeviceWidget2::resetReceiveDataCount()
{
    ui->labelReceiveBytes->setText("0");
    ui->labelReceiveFrames->setText("0");
}

void SAKIODeviceWidget2::setOutputMode(QString mode)
{
    QString option = QString("OutputMode");
    writeSetting(option, mode);
}

void SAKIODeviceWidget2::readOutputMode()
{
    QString option = QString("OutputMode");
    QString value = readSetting(option);

    /// 默认使用Ascii输入
    if (value.isEmpty()){
        value = QString(textModel.valueToKey(Ascii));
    }

    ui->comboBoxOutputMode->setCurrentText(value);
}

void SAKIODeviceWidget2::setInputMode(QString mode)
{
    QString option = QString("InputMode");
    writeSetting(option, mode);
    inputTextMode = mode;
}

void SAKIODeviceWidget2::readInputMode()
{
    QString option = QString("InputMode");
    QString value = readSetting(option);

    /// 默认使用Ascii输入
    if (value.isEmpty()){
        value = QString(textModel.valueToKey(Ascii));
    }

    inputTextMode = value;
    textFormatControl();

    ui->comboBoxInputMode->setCurrentText(value);
}

void SAKIODeviceWidget2::updateTxImage()
{
    static bool b = false;
    if (b){
        ui->labelTX->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }else{
        ui->labelTX->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }

    b = !b;
}

void SAKIODeviceWidget2::updateRxImage()
{
    static bool b = false;
    if (b){
        ui->labelRX->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }else{
        ui->labelRX->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }

    b = !b;
}

void SAKIODeviceWidget2::setDelayTime(QString time)
{
    QString option = QString("DelayTime");
    QString value = time;
    writeSetting(option, value);
}

void SAKIODeviceWidget2::readDelayTime()
{
    QString option = QString("DelayTime");
    QString value = readSetting(option);

    ui->lineEditBytesDelayTime->setText(value);
}

void SAKIODeviceWidget2::setCycleTime(QString time)
{
    QString option = QString("CycleTime");
    writeSetting(option, time);
}

void SAKIODeviceWidget2::readCycleTime()
{
    QString option = QString("CycleTime");
    QString value = readSetting(option);

    ui->lineEditCycleTime->setText(value);
}

void SAKIODeviceWidget2::setAutoResponseFlag(bool enableAutoResponse)
{
    QString option = QString("AutoResponse");
    QString value = enableAutoResponse ? QString("true") : QString("false");

    writeSetting(option, value);
}

void SAKIODeviceWidget2::readAutoResponseFlag()
{

}

void SAKIODeviceWidget2::handleReadBytes(QByteArray data)
{
    Q_UNUSED(data);
}

QString SAKIODeviceWidget2::readSetting(QString &option)
{
    QSettings settings;
    QString key = device->deviceName() + "/" + option;
    QString value = settings.value(key).toString();

    return value;
}

void SAKIODeviceWidget2::writeSetting(QString &option, QString &value)
{
    QSettings settings;
    QString key = device->deviceName() + "/" + option;

    settings.setValue(key, value);
}

void SAKIODeviceWidget2::initUiPointer()
{
    /*
     * 设备管理组
     */
    refreshPushButton   = ui->pushButtonRefresh;
    switchPushButton    = ui->pushButtonOpen;
    rxLabel             = ui->labelRX;
    txLabel             = ui->labelTX;

    /*
     * 输入设置组
     */
    inputModelComboBox      = ui->comboBoxInputMode;
    cycleEnableCheckBox     = ui->checkBoxCycle;
    cycleTimeLineEdit       = ui->lineEditCycleTime;
    readinFilePushButton    = ui->pushButtonReadInFile;
    clearInputPushButton    = ui->pushButtonClearInput;
    sendPushButton          = ui->pushButtonSendData;
    inputTextEdit           = ui->textEditInputData;

    /*
     * 输出设置组
     */
    outputModelComboBox     = ui->comboBoxOutputMode;
    showDateCheckBox        = ui->checkBoxOutputDate;
    showTimeCheckBox        = ui->checkBoxOutputTime;
    showMsCheckBox          = ui->checkBoxOutputMS;
    showRxDataCheckBox      = ui->checkBoxShowRxOutput;
    showTxDataCheckBox      = ui->checkBoxShowTxOutput;
    clearOutputPushButton   = ui->pushButtonClearOutput;
    saveOutputPushButton    = ui->pushButtonSaveOutput;
    outputTextBroswer       = ui->textBrowserOutputData;

    /*
     * 数据管理组
     */
    rxFramesLabel           = ui->labelReceiveFrames;
    txFramesLabel           = ui->labelSendFrames;
    rxBytesLabel            = ui->labelReceiveBytes;
    txBytesLabel            = ui->labelSendBytes;
    resetRxCountPushButton  = ui->pushButtonResetReceiveDataCount;
    resetTxCountPushButton  = ui->pushButtonResetSendDataCount;

    /*
     * 其他设计组
     */
    autoResponseSettingPushButton  = ui->pushButtonAutoRespone;
    highlightSettingPushButton     = ui->pushButtonHighlighting;
    readWriteSettingPushButton     = ui->pushButtonRWParameter;

    connect(autoResponseSettingPushButton, &QPushButton::clicked, this, &SAKIODeviceWidget2::createAutoResponseSettingWindow);
    connect(highlightSettingPushButton,    &QPushButton::clicked, this, &SAKIODeviceWidget2::createHighlightSettingWindow);
    connect(readWriteSettingPushButton,    &QPushButton::clicked, this, &SAKIODeviceWidget2::createReadWriteSettingWindow);
}

void SAKIODeviceWidget2::createAutoResponseSettingWindow()
{
    SAKAutoResponseSettingPanel *window = new SAKAutoResponseSettingPanel(this);
    window->show();
}

void SAKIODeviceWidget2::createHighlightSettingWindow()
{
    SAKHighlighterSettingPanel *window = new SAKHighlighterSettingPanel(outputTextBroswer->document(), this);
    window->show();
}

void SAKIODeviceWidget2::createReadWriteSettingWindow()
{
    SAKReadWriteSetting *window = new SAKReadWriteSetting(this);
    window->show();
}
