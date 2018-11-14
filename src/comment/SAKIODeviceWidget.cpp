/*******************************************************************************
* The file is encoding with utf-8 (with BOM)
*
* I write the comment with English, it's not because that I'm good at English,
* but for "installing B".
*
* Copyright (C) 2018-2018 wuhai persionnal. All rights reserved.
*******************************************************************************/
#if _MSC_VER > 1600
#pragma execution_character_set("utf-8")
#endif

#include <QDateTime>
#include <QIntValidator>
#include <QScrollBar>
#include <QFileDialog>
#include <QTextStream>
#include <QPixmap>

#include "SAKIODeviceWidget.h"
#include "SAKSettings.h"

#include "ui_SAKIODeviceWidget.h"

SAKIODeviceWidget::SAKIODeviceWidget(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent)
    :QWidget(parent)
    ,device(_device)
    ,controler(_controler)
    ,autoResponseSettingPanel(new AutoResponseSettingPanel)
    ,cycleTimer(new QTimer)
    ,customControlerLayout(new QHBoxLayout)
    ,delayTimer(new QTimer)
    ,ui(new Ui::SAKIODeviceWidget)
{
    ui->setupUi(this);
    Connect();
    setCustomControler(controler);
    initUI();
}

SAKIODeviceWidget::~SAKIODeviceWidget()
{
    delete ui;
}

void SAKIODeviceWidget::initUI()
{
    ui->checkBoxOutputMS->setEnabled(true);
    ui->checkBoxOutputDate->setChecked(true);
    ui->checkBoxOutputTime->setChecked(true);
    ui->lineEditCycleTime->setValidator(new QIntValidator(10, 24*60*60*1000, ui->lineEditCycleTime));
    ui->lineEditBytesDelayTime->setValidator(new QIntValidator(10, 24*60*60*1000, ui->lineEditBytesDelayTime));

#if 0
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
    ui->labelInfoIcon->setPixmap(QPixmap(":/images/info.png").scaled(rxtxSize, Qt::KeepAspectRatio));
}

void SAKIODeviceWidget::outputTimeInfoCheckBoxClicked(bool checked)
{
    if (checked){
        ui->checkBoxOutputMS->setEnabled(true);
    }else {
        ui->checkBoxOutputMS->setChecked(false);
        ui->checkBoxOutputMS->setEnabled(false);
    }
}

void SAKIODeviceWidget::outputReceiveDataOnlyCheckBoxClicked(bool cheaked)
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
        ui->checkBoxOutputDate->setChecked(true);
        ui->checkBoxOutputDate->setEnabled(true);
        ui->checkBoxOutputTime->setChecked(true);
        ui->checkBoxOutputTime->setEnabled(true);
        ui->checkBoxOutputMS->setChecked(false);
        ui->checkBoxOutputMS->setEnabled(true);
    }
}

void SAKIODeviceWidget::openOrClose()
{
    if (device->isOpen()){
        close();
    }else{
        open();
    }
}

void SAKIODeviceWidget::setCustomControler(SAKIODeviceControler *controler)
{
    customControlerLayout->addWidget(controler);
    customControlerLayout->setMargin(0);
    ui->groupBoxCustom->setLayout(customControlerLayout);
}

void SAKIODeviceWidget::setRefreshPBtText(QString text)
{
    ui->pushButtonRefresh->setText(text);
}

void SAKIODeviceWidget::Connect()
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
    connect(device, SIGNAL(bytesWritten(qint64)), this, SLOT(bytesWritten(qint64)));

    /// 重置收发计数
    connect(ui->pushButtonResetReceiveDataCount, SIGNAL(clicked(bool)), this, SLOT(resetReceiveDataCount()));
    connect(ui->pushButtonResetSendDataCount, SIGNAL(clicked(bool)), this, SLOT(resetSendDataCount()));

    /// 循环周期
    connect(ui->lineEditCycleTime, SIGNAL(textChanged(QString)), this, SLOT(setCycleTime(QString)));
    /// 字节间延时
    connect(ui->lineEditBytesDelayTime, SIGNAL(textChanged(QString)), this, SLOT(setDelayTime(QString)));

    /// 指示灯
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(updateRxImage()));
    connect(device, SIGNAL(bytesWritten(qint64)),  this, SLOT(updateTxImage()));

    /// 文本模式
    connect(ui->comboBoxInputMode, SIGNAL(currentIndexChanged(int)), this , SLOT(setInputMode(int)));
    connect(ui->comboBoxOutputMode, SIGNAL(currentIndexChanged(int)), this, SLOT(setOutputMode(int)));

    /// 循环发送与字节间延时发送
    connect(ui->checkBoxBytesDelay, SIGNAL(clicked(bool)), this, SLOT(cancleCycle()));
    connect(ui->checkBoxCycle, SIGNAL(clicked(bool)), this, SLOT(cancleBytesDelay()));

    /// 弹出自动回复设置面板
    connect(ui->pushButtonAutoRespone, SIGNAL(clicked(bool)), autoResponseSettingPanel, SLOT(show()));

    /// 自动回复
    connect(autoResponseSettingPanel, SIGNAL(autoResponseFlagChanged(bool)), this, SLOT(setAutoResponseFlag(bool)));
}

void SAKIODeviceWidget::afterDeviceOpen()
{
    controler->afterDeviceOpen();
    ui->pushButtonRefresh->setEnabled(false);
    ui->pushButtonOpen->setText(tr("关闭"));
}

void SAKIODeviceWidget::afterDeviceClose()
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

void SAKIODeviceWidget::outputInfo(QString info, QString color, bool prefix)
{
    QString strTemp;
    if (prefix){
        strTemp += QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        strTemp = QString("<font color=silver>%1 </font>").arg(strTemp);
    }

    strTemp += QString("<font color=%1>%2</font>").arg(color).arg(info);
    ui->labelOutput->setText(strTemp);
}

void SAKIODeviceWidget::outputErrorString(QString str)
{
    outputInfo(str, "red");
}

void SAKIODeviceWidget::outputInformationString(QString str)
{
    outputInfo(str, "green");
}

void SAKIODeviceWidget::writeBytes()
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

void SAKIODeviceWidget::checkedBoxCycleClicked(bool checked)
{
    if (checked){

    }else{
        cycleTimer->stop();
        ui->pushButtonSendData->setEnabled(true);
        ui->lineEditCycleTime->setEnabled(true);
    }
}

void SAKIODeviceWidget::checkedBoxDelayClicked(bool checked)
{
    if (checked){

    }else {
        delayTimer->stop();
        ui->pushButtonSendData->setEnabled(true);
        ui->lineEditCycleTime->setEnabled(true);
    }
}

void SAKIODeviceWidget::cycleTimerTimeout()
{
    if (device->isOpen()){
        emit need2writeBytes(dataBytes());
    }else {
        cycleTimer->stop();
        outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
    }
}

void SAKIODeviceWidget::delayTimerTimeout()
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

void SAKIODeviceWidget::cancleBytesDelay()
{
    ui->checkBoxBytesDelay->setChecked(false);
}

void SAKIODeviceWidget::cancleCycle()
{
    ui->checkBoxCycle->setChecked(false);
}

QByteArray SAKIODeviceWidget::dataBytes()
{
    QByteArray data;

    QString str = ui->textEditInputData->toPlainText();
    if (str.isEmpty()){
        str = "(null)";
        data = str.toLatin1();
        return data;
    }

    if (inputTextMode == TextModeBin){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 2)));
        }
    }else if (inputTextMode == TextModeOct){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 8)));
        }
    }else if (inputTextMode == TextModeDec){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 10)));
        }
    }else if (inputTextMode == TextModeHex){
        QStringList strList = str.split(' ');
        foreach (QString str, strList){
            data.append(static_cast<int8_t>(QString(str).toInt(nullptr, 16)));
        }
    }else if (inputTextMode == TextModeAscii){
        data = ui->textEditInputData->toPlainText().toLatin1();
    }else if (inputTextMode == TextModeUtf8){
        data = ui->textEditInputData->toPlainText().toLocal8Bit();
    }else {
        qWarning() << tr("未知输入各式！");
    }

    return data;
}

void SAKIODeviceWidget::textFormatControl()
{
    disconnect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
    QString plaintext = ui->textEditInputData->toPlainText();
    if (plaintext.isEmpty()){
        connect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
        return;
    }else {
        if (inputTextMode == TextModeBin){  /// 二进制输入
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
        }else if(inputTextMode == TextModeOct) { /// 八进制输入
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
        }else if(inputTextMode == TextModeDec) { /// 十进制
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
        }else if(inputTextMode == TextModeHex) { /// 十六进制
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
        }else if(inputTextMode == TextModeAscii) { /// ascii
            plaintext.remove(QRegExp("[^\0u00-\u007f ]"));
            ui->textEditInputData->setText(plaintext);
            ui->textEditInputData->moveCursor(QTextCursor::End);
        }else if(inputTextMode == TextModeUtf8) {    /// utf-8
            /// nothing to do
        }else {
            qWarning() << tr("未知输入选项！");
        }
    }
    connect(ui->textEditInputData, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
}

void SAKIODeviceWidget::bytesRead(QByteArray data)
{
    /// 更新接收统计
    qlonglong receiveFrameCount = ui->labelReceiveFrames->text().toLongLong();
    receiveFrameCount += 1;
    ui->labelReceiveFrames->setText(QString::number(receiveFrameCount));
    qlonglong receiveBytesCount = ui->labelReceiveBytes->text().toLongLong();
    receiveBytesCount += data.length();
    ui->labelReceiveBytes->setText(QString::number(receiveBytesCount));

    if (ui->checkBoxPause->isChecked()){
        return;
    }

    QString str;
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

    if (outputTextMode == TextModeBin){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 2), 8, '0'));
        }
    }else if (outputTextMode == TextModeOct){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 8), 8, '0'));
        }
    }else if (outputTextMode == TextModeDec){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 10), 8, '0'));
        }
    }else if (outputTextMode == TextModeHex){
        for (int i = 0; i < data.length(); i++){
            str.append(QString("%1 ").arg(QString::number(static_cast<uint8_t>(data.at(i)), 16), 8, '0'));
        }
    }else if (outputTextMode == TextModeAscii){
        str.append(QString(data));
    }else if (outputTextMode == TextModeUtf8){
        str.append(QString(data));
    }else {
        qWarning() << "Unknown output model!";
    }

    if (ui->checkBoxOutputReceiveDataOnly->isChecked()){
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

void SAKIODeviceWidget::openFile()
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

void SAKIODeviceWidget::saveOutputData()
{
    QString outFileName = QFileDialog::getSaveFileName();
    QFile outFile(outFileName);

    outFile.open(QIODevice::WriteOnly|QIODevice::Text);

    QTextStream outStream(&outFile);
    outStream << ui->textBrowserOutputData->toPlainText();

    outFile.flush();
    outFile.close();
}

void SAKIODeviceWidget::bytesWritten(qint64 bytes)
{
    qlonglong writeBytes = ui->labelSendBytes->text().toLongLong();
    writeBytes += bytes;
    ui->labelSendBytes->setText(QString::number(writeBytes));

    qlonglong writeFrame = ui->labelSendFrames->text().toLongLong();
    writeFrame += 1;
    ui->labelSendFrames->setText(QString::number(writeFrame));
}

void SAKIODeviceWidget::resetSendDataCount()
{
    ui->labelSendBytes->setText("0");
    ui->labelSendFrames->setText("0");
}

void SAKIODeviceWidget::resetReceiveDataCount()
{
    ui->labelReceiveBytes->setText("0");
    ui->labelReceiveFrames->setText("0");
}

void SAKIODeviceWidget::setOutputMode(int mode)
{
    if (sakSettings() == nullptr){
        qWarning() << "The settings function is not initialized yet!";
        return;
    }else{
        outputTextMode = static_cast<TextMode>(mode);

        if (outputTextMode == TextModeBin){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowOutputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportOutputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientOutputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientOutputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerOutputMode(MODEBIN);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (outputTextMode == TextModeOct){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowOutputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportOutputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientOutputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientOutputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerOutputMode(MODEOCT);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (outputTextMode == TextModeDec){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowOutputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportOutputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientOutputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientOutputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerOutputMode(MODEDEC);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (outputTextMode == TextModeHex){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowOutputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportOutputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientOutputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientOutputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerOutputMode(MODEHEX);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (outputTextMode == TextModeAscii){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowOutputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportOutputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientOutputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientOutputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerOutputMode(MODEASCII);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (outputTextMode == TextModeUtf8){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowOutputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportOutputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientOutputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientOutputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerOutputMode(MODEUTF8);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else {
            qWarning("Unknow error!");
        }
    }
}



void SAKIODeviceWidget::readOutputMode()
{
    QString tip = QString("Unknow output mode!");

    if (sakSettings() == nullptr){
        qWarning() << "The settings function is not initialized yet!";
        return;
    }else {
        if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
            if (sakSettings()->valueUnknowOutputMode().compare(MODEBIN) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeBin);
                outputTextMode = TextModeBin;
            }else if (sakSettings()->valueUnknowOutputMode().compare(MODEOCT) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeOct);
                outputTextMode = TextModeOct;
            }else if (sakSettings()->valueUnknowOutputMode().compare(MODEDEC) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeDec);
                outputTextMode = TextModeDec;
            }else if (sakSettings()->valueUnknowOutputMode().compare(MODEHEX) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeHex);
                outputTextMode = TextModeHex;
            }else if (sakSettings()->valueUnknowOutputMode().compare(MODEASCII) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeAscii);
                outputTextMode = TextModeAscii;
            }else if (sakSettings()->valueUnknowOutputMode().compare(MODEUTF8) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeUtf8);
                outputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
            if (sakSettings()->valueSerialportOutputMode().compare(MODEBIN) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeBin);
                outputTextMode = TextModeBin;
            }else if (sakSettings()->valueSerialportOutputMode().compare(MODEOCT) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeOct);
                outputTextMode = TextModeOct;
            }else if (sakSettings()->valueSerialportOutputMode().compare(MODEDEC) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeDec);
                outputTextMode = TextModeDec;
            }else if (sakSettings()->valueSerialportOutputMode().compare(MODEHEX) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeHex);
                outputTextMode = TextModeHex;
            }else if (sakSettings()->valueSerialportOutputMode().compare(MODEASCII) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeAscii);
                outputTextMode = TextModeAscii;
            }else if (sakSettings()->valueSerialportOutputMode().compare(MODEUTF8) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeUtf8);
                outputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
            if (sakSettings()->valueUdpClientOutputMode().compare(MODEBIN) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeBin);
                outputTextMode = TextModeBin;
            }else if (sakSettings()->valueUdpClientOutputMode().compare(MODEOCT) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeOct);
                outputTextMode = TextModeOct;
            }else if (sakSettings()->valueUdpClientOutputMode().compare(MODEDEC) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeDec);
                outputTextMode = TextModeDec;
            }else if (sakSettings()->valueUdpClientOutputMode().compare(MODEHEX) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeHex);
                outputTextMode = TextModeHex;
            }else if (sakSettings()->valueUdpClientOutputMode().compare(MODEASCII) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeAscii);
                outputTextMode = TextModeAscii;
            }else if (sakSettings()->valueUdpClientOutputMode().compare(MODEUTF8) == 0){
                 ui->comboBoxOutputMode->setCurrentIndex(TextModeUtf8);
                 outputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
            if (sakSettings()->valueTcpClientOutputMode().compare(MODEBIN) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeBin);
                outputTextMode = TextModeBin;
            }else if (sakSettings()->valueTcpClientOutputMode().compare(MODEOCT) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeOct);
                outputTextMode = TextModeOct;
            }else if (sakSettings()->valueTcpClientOutputMode().compare(MODEDEC) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeDec);
                outputTextMode = TextModeDec;
            }else if (sakSettings()->valueTcpClientOutputMode().compare(MODEHEX) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeHex);
                outputTextMode = TextModeHex;
            }else if (sakSettings()->valueTcpClientOutputMode().compare(MODEASCII) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeAscii);
                outputTextMode = TextModeAscii;
            }else if (sakSettings()->valueTcpClientOutputMode().compare(MODEUTF8) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeUtf8);
                outputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
            if (sakSettings()->valueTcpServerOutputMode().compare(MODEBIN) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeBin);
                outputTextMode = TextModeBin;
            }else if (sakSettings()->valueTcpServerOutputMode().compare(MODEOCT) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeOct);
                outputTextMode = TextModeOct;
            }else if (sakSettings()->valueTcpServerOutputMode().compare(MODEDEC) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeDec);
                outputTextMode = TextModeDec;
            }else if (sakSettings()->valueTcpServerOutputMode().compare(MODEHEX) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeHex);
                outputTextMode = TextModeHex;
            }else if (sakSettings()->valueTcpServerOutputMode().compare(MODEASCII) == 0){
                ui->comboBoxOutputMode->setCurrentIndex(TextModeAscii);
                outputTextMode = TextModeAscii;
            }else if (sakSettings()->valueTcpServerOutputMode().compare(MODEUTF8) == 0){
                 ui->comboBoxOutputMode->setCurrentIndex(TextModeUtf8);
                 outputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else {
            qWarning("Unknow device type!");
        }
    }
}

void SAKIODeviceWidget::setInputMode(int mode)
{
    if (sakSettings() == nullptr){
        qWarning() << "The settings function is not initialized yet!";
        return;
    }else {
        inputTextMode = static_cast<TextMode>(mode);
        ui->textEditInputData->clear();

        if (inputTextMode == TextModeBin){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowInputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportInputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientInputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientInputMode(MODEBIN);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerInputMode(MODEBIN);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (inputTextMode == TextModeOct){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowInputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportInputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientInputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientInputMode(MODEOCT);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerInputMode(MODEOCT);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (inputTextMode == TextModeDec){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowInputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportInputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientInputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientInputMode(MODEDEC);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerInputMode(MODEDEC);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (inputTextMode == TextModeHex){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowInputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportInputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientInputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientInputMode(MODEHEX);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerInputMode(MODEHEX);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (inputTextMode == TextModeAscii){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowInputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportInputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientInputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientInputMode(MODEASCII);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerInputMode(MODEASCII);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else if (inputTextMode == TextModeUtf8){
            if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
                sakSettings()->setValueUnknowInputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
                sakSettings()->setValueSerialportInputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
                sakSettings()->setValueUdpClientInputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
                sakSettings()->setValueTcpClientInputMode(MODEUTF8);
            }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
                sakSettings()->setValueTcpServerInputMode(MODEUTF8);
            }else {
                qWarning() << "Unknow device type!";
            }
        }else {
            qWarning("Unknow error!");
        }
    }
}

void SAKIODeviceWidget::readInputMode()
{
    QString tip = QString("Unknow input mode!");

    if (sakSettings() == nullptr){
        qWarning() << "The settings function is not initialized yet!";
        return;
    }else {
        if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
            if (sakSettings()->valueUnknowInputMode().compare(MODEBIN) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeBin);
                inputTextMode = TextModeBin;
            }else if (sakSettings()->valueUnknowInputMode().compare(MODEOCT) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeOct);
                inputTextMode = TextModeOct;
            }else if (sakSettings()->valueUnknowInputMode().compare(MODEDEC) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeDec);
                inputTextMode = TextModeDec;
            }else if (sakSettings()->valueUnknowInputMode().compare(MODEHEX) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeHex);
                inputTextMode = TextModeHex;
            }else if (sakSettings()->valueUnknowInputMode().compare(MODEASCII) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeAscii);
                inputTextMode = TextModeAscii;
            }else if (sakSettings()->valueUnknowInputMode().compare(MODEUTF8) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeUtf8);
                inputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
            if (sakSettings()->valueSerialportInputMode().compare(MODEBIN) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeBin);
                inputTextMode = TextModeBin;
            }else if (sakSettings()->valueSerialportInputMode().compare(MODEOCT) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeOct);
                inputTextMode = TextModeOct;
            }else if (sakSettings()->valueSerialportInputMode().compare(MODEDEC) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeDec);
                inputTextMode = TextModeDec;
            }else if (sakSettings()->valueSerialportInputMode().compare(MODEHEX) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeHex);
                inputTextMode = TextModeHex;
            }else if (sakSettings()->valueSerialportInputMode().compare(MODEASCII) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeAscii);
                inputTextMode = TextModeAscii;
            }else if (sakSettings()->valueSerialportInputMode().compare(MODEUTF8) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeUtf8);
                inputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
            if (sakSettings()->valueUdpClientInputMode().compare(MODEBIN) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeBin);
                inputTextMode = TextModeBin;
            }else if (sakSettings()->valueUdpClientInputMode().compare(MODEOCT) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeOct);
                inputTextMode = TextModeOct;
            }else if (sakSettings()->valueUdpClientInputMode().compare(MODEDEC) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeDec);
                inputTextMode = TextModeDec;
            }else if (sakSettings()->valueUdpClientInputMode().compare(MODEHEX) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeHex);
                inputTextMode = TextModeHex;
            }else if (sakSettings()->valueUdpClientInputMode().compare(MODEASCII) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeAscii);
                inputTextMode = TextModeAscii;
            }else if (sakSettings()->valueUdpClientInputMode().compare(MODEUTF8) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeUtf8);
                inputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
            if (sakSettings()->valueTcpClientInputMode().compare(MODEBIN) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeBin);
                inputTextMode = TextModeBin;
            }else if (sakSettings()->valueTcpClientInputMode().compare(MODEOCT) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeOct);
                inputTextMode = TextModeOct;
            }else if (sakSettings()->valueTcpClientInputMode().compare(MODEDEC) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeDec);
                inputTextMode = TextModeDec;
            }else if (sakSettings()->valueTcpClientInputMode().compare(MODEHEX) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeHex);
                inputTextMode = TextModeHex;
            }else if (sakSettings()->valueTcpClientInputMode().compare(MODEASCII) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeAscii);
                inputTextMode = TextModeAscii;
            }else if (sakSettings()->valueTcpClientInputMode().compare(MODEUTF8) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeUtf8);
                inputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
            if (sakSettings()->valueTcpServerInputMode().compare(MODEBIN) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeBin);
                inputTextMode = TextModeBin;
            }else if (sakSettings()->valueTcpServerInputMode().compare(MODEOCT) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeOct);
                inputTextMode = TextModeOct;
            }else if (sakSettings()->valueTcpServerInputMode().compare(MODEDEC) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeDec);
                inputTextMode = TextModeDec;
            }else if (sakSettings()->valueTcpServerInputMode().compare(MODEHEX) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeHex);
                inputTextMode = TextModeHex;
            }else if (sakSettings()->valueTcpServerInputMode().compare(MODEASCII) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeAscii);
                inputTextMode = TextModeAscii;
            }else if (sakSettings()->valueTcpServerInputMode().compare(MODEUTF8) == 0){
                ui->comboBoxInputMode->setCurrentIndex(TextModeUtf8);
                inputTextMode = TextModeUtf8;
            }else {
                qWarning() << tip;
            }
        }else {
            qWarning("Unknow device type!");
        }
    }
}

void SAKIODeviceWidget::setCycleTime(QString time)
{
    if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
        sakSettings()->setValueUnknowCycleTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
        sakSettings()->setValueSerialportCycleTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
        sakSettings()->setValueUdpClientCycleTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
        sakSettings()->setValueTcpClientCycleTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
        sakSettings()->setValueTcpServerCycleTime(time);
    }else {
        qWarning() << "Unknow device type!";
    }
}

void SAKIODeviceWidget::readCycleTime()
{
    if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
        ui->lineEditCycleTime->setText(sakSettings()->valueUnknowCycleTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
        ui->lineEditCycleTime->setText(sakSettings()->valueSerialportCycleTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
        ui->lineEditCycleTime->setText(sakSettings()->valueUdpClientCycleTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
        ui->lineEditCycleTime->setText(sakSettings()->valueTcpClientCycleTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
        ui->lineEditCycleTime->setText(sakSettings()->valueTcpServerCycleTime());
    }else {
        ui->lineEditCycleTime->setText("1000");
    }
}

void SAKIODeviceWidget::updateTxImage()
{
    static bool b = false;
    if (b){
        ui->labelTX->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }else{
        ui->labelTX->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }

    b = !b;
}

void SAKIODeviceWidget::updateRxImage()
{
    static bool b = false;
    if (b){
        ui->labelRX->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }else{
        ui->labelRX->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }

    b = !b;
}

void SAKIODeviceWidget::setDelayTime(QString time)
{
    if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
        sakSettings()->setValueUnknowDelayTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
        sakSettings()->setValueSerialportDelayTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
        sakSettings()->setValueUdpClientDelayTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
        sakSettings()->setValueTcpClientDelayTime(time);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
        sakSettings()->setValueTcpServerDelayTime(time);
    }else {
        qWarning() << "Unknow device type!";
    }
}

void SAKIODeviceWidget::readDelayTime()
{
    if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
        ui->lineEditBytesDelayTime->setText(sakSettings()->valueUnknowDelayTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
        ui->lineEditBytesDelayTime->setText(sakSettings()->valueSerialportDelayTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
        ui->lineEditBytesDelayTime->setText(sakSettings()->valueUdpClientDelayTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
        ui->lineEditBytesDelayTime->setText(sakSettings()->valueTcpClientDelayTime());
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
        ui->lineEditBytesDelayTime->setText(sakSettings()->valueTcpServerDelayTime());
    }else {
        ui->lineEditBytesDelayTime->setText("1000");
    }
}

void SAKIODeviceWidget::setAutoResponseFlag(bool enableAutoResponse)
{
    if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
        sakSettings()->setValueUnknowAutoResponseFlag(enableAutoResponse ? "true" : "false");
    }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
        sakSettings()->setvalueSerialportAutoResponseFlag(enableAutoResponse ? "true" : "false");
    }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
        sakSettings()->setValueUdpClientAutoResponseFlag(enableAutoResponse ? "true" : "false");
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
        sakSettings()->setValueTcpClientAutoResponseFlag(enableAutoResponse ? "true" : "false");
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
        sakSettings()->setvalueTcpServerAutoResponseFlag(enableAutoResponse ? "true" : "false");
    }else {
        qWarning() << "Unknow device type!";
    }
}

void SAKIODeviceWidget::readAutoResponseFlag()
{
    if (device->deviceType() == SAKIODevice::SAKDeviceUnknow){
        autoResponseSettingPanel->setAutoResponseFlag((sakSettings()->valueUnknowAutoResponseFlag().compare("true") == 0) ? true : false);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceSerialport){
        autoResponseSettingPanel->setAutoResponseFlag((sakSettings()->valueSerialportAutoResponseFlag().compare("true") == 0) ? true : false);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceUdp){
        autoResponseSettingPanel->setAutoResponseFlag((sakSettings()->valueUdpClientAutoResponseFlag().compare("true") == 0) ? true : false);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcp){
        autoResponseSettingPanel->setAutoResponseFlag((sakSettings()->valueTcpClientAutoResponseFlag().compare("true") == 0) ? true : false);
    }else if (device->deviceType() == SAKIODevice::SAKDeviceTcpServer){
        autoResponseSettingPanel->setAutoResponseFlag((sakSettings()->valueTcpServerAutoResponseFlag().compare("true") == 0) ? true : false);
    }else {
        autoResponseSettingPanel->setAutoResponseFlag(false);
    }
}
