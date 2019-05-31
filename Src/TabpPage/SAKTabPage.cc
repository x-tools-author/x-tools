/*
 * The file is encoding with utf-8 (with BOM)
 *
 * I write the comment with English, it's not because that I'm good at English,
 * but for "installing B".
 *
 * Copyright (C) 2018-2019 woohii persionnal. No rights reserved.
 */
#include <QDebug>
#include <QDateTime>
#include <QIntValidator>
#include <QScrollBar>
#include <QFileDialog>
#include <QTextStream>
#include <QPixmap>
#include <QSettings>
#include <QKeyEvent>
#include <QMetaEnum>
#include <QLoggingCategory>

#include "SAKTabPage.hh"
#include "SAKHighlighterSettingPanel.hh"

#include "ui_SAKTabPage.h"

SAKTabPage::SAKTabPage(SAKIODevice *_device, SAKIODeviceControler *_controler, QWidget *parent)
    :QWidget(parent)
    ,ui(new Ui::SAKTabPage)
    ,device (_device)
    ,controler (_controler)                  
    ,autoResponseSettingPanel (new SAKAutoResponseSettingPanel)
    ,cycleTimer (new QTimer)
    ,customControlerLayout (new QHBoxLayout)
    ,clearInfoTimer (new QTimer)
{
    ui->setupUi(this);
    initUiPointer();

    inputTextModelComboBox = inputModelComboBox;
    outputTextModelComboBox = outputModelComboBox;

    rwParameterSettingButton = readWriteSettingPushButton;


    /// 初始化输入输出模式
    inputTextModelComboBox->clear();
    outputTextModelComboBox->clear();
    textModel = QMetaEnum::fromType<SAKTabPage::TextDisplayModel>();
    for (int i = 0; i < textModel.keyCount(); i++){
        inputTextModelComboBox->addItem(QString(textModel.valueToKey(i)));
        outputTextModelComboBox->addItem(QString(textModel.valueToKey(i)));
    }

    /// 初始化数据成员
    highlighterSettingButton = highlightSettingPushButton;
    highlighterSettingPanel = new SAKHighlighterSettingPanel(outputTextBroswer->document());

    Connect();
    setCustomControler(controler);
    initUI();

    clearInfoTimer->setInterval(3*1000);
    connect(clearInfoTimer, SIGNAL(timeout()), this, SLOT(clearInfo()));
}

SAKTabPage::~SAKTabPage()
{
    delete ui;
    delete logCategory;
}

void SAKTabPage::initUI()
{
    showMsCheckBox->setEnabled(true);
    showDateCheckBox->setChecked(false);
    showTimeCheckBox->setChecked(true);
    cycleTimeLineEdit->setValidator(new QIntValidator(10, 24*60*60*1000, cycleTimeLineEdit));

#if 0   /// 按钮图标
    clearInputPushButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    clearOutputPushButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DialogResetButton));
    readinFilePushButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    saveOutputPushButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_DirOpenIcon));
    refreshPushButton->setIcon(QApplication::style()->standardIcon(QStyle::SP_BrowserReload));
#endif

    readOutputMode();
    readInputMode();
    readCycleTime();
    readAutoResponseFlag();

    rxLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    txLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
//    ui->labelInfoIcon->setPixmap(QPixmap(":/images/Info.png").scaled(QSize(ui->labelInfoIcon->width()-4,
//                                                                           ui->labelInfoIcon->height() - 4),
//                                                                     Qt::KeepAspectRatio));

    outputTextBroswer->setLineWrapMode(QTextEdit::WidgetWidth);
}

void SAKTabPage::outputTimeInfoCheckBoxClicked(bool checked)
{
    if (checked){
        showMsCheckBox->setEnabled(true);
    }else {
        showMsCheckBox->setChecked(false);
        showMsCheckBox->setEnabled(false);
    }
}

void SAKTabPage::outputReceiveDataOnlyCheckBoxClicked(bool cheaked)
{
    outputTextBroswer->clear();

    if (cheaked){
        showDateCheckBox->setChecked(false);
        showDateCheckBox->setEnabled(false);
        showTimeCheckBox->setChecked(false);
        showTimeCheckBox->setEnabled(false);
        showMsCheckBox->setChecked(false);
        showMsCheckBox->setEnabled(false);
    }else{
        showDateCheckBox->setChecked(false);
        showDateCheckBox->setEnabled(true);
        showTimeCheckBox->setChecked(true);
        showTimeCheckBox->setEnabled(true);
        showMsCheckBox->setChecked(false);
        showMsCheckBox->setEnabled(true);
    }
}

void SAKTabPage::openOrClose()
{
    if (device->isOpen()){
        close();
    }else{
        open();
    }
}

void SAKTabPage::setCustomControler(SAKIODeviceControler *controler)
{
    customControlerLayout->addWidget(controler);
    customControlerLayout->setMargin(0);
    ui->groupBoxCustom->setLayout(customControlerLayout);
}

void SAKTabPage::setRefreshPBtText(QString text)
{
    refreshPushButton->setText(text);
}

void SAKTabPage::Connect()
{
    /// 打开/关闭设备
    connect(switchPushButton, SIGNAL(clicked(bool)), this, SLOT(openOrClose()));
    connect(this, SIGNAL(need2open()), controler, SLOT(open()));
    connect(this, SIGNAL(need2close()), device, SLOT(close()));
    connect(controler, SIGNAL(need2open(QString,QString)), device, SLOT(open(QString,QString)));
    connect(controler, SIGNAL(need2open(QString,QString,QString,QString)), device, SLOT(open(QString,QString,QString,QString)));
    connect(controler, SIGNAL(need2open(QString,QString,QString,QString,QString)), device, SLOT(open(QString,QString,QString,QString,QString)));
    connect(device, SIGNAL(deviceOpenSuccessfully()), this, SLOT(afterDeviceOpen()));
    connect(device, SIGNAL(deviceCloseSuccessfully()), this, SLOT(afterDeviceClose()));

    /// 刷新设备
    connect(refreshPushButton, SIGNAL(clicked(bool)), controler, SLOT(refresh()));

    connect(cycleTimer, SIGNAL(timeout()), this, SLOT(cycleTimerTimeout()));
    connect(cycleEnableCheckBox, SIGNAL(clicked(bool)), this, SLOT(checkedBoxCycleClicked(bool)));
    connect(device, SIGNAL(errorStr(QString)), this, SLOT(outputErrorString(QString)));
    connect(device, SIGNAL(infoStr(QString)), this, SLOT(outputInformationString(QString)));
    connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));

    connect(showTimeCheckBox, SIGNAL(clicked(bool)), this, SLOT(outputTimeInfoCheckBoxClicked(bool)));

    /// 数据收发
    connect(sendPushButton, SIGNAL(clicked(bool)), this, SLOT(writeBytes()));
    connect(this, SIGNAL(need2writeBytes(QByteArray)), device, SLOT(writeBytes(QByteArray)));
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(bytesRead(QByteArray)));
    connect(device, SIGNAL(bytesWritten(QByteArray)), this, SLOT(bytesWritten(QByteArray)));

    /// 重置收发计数
    connect(resetRxCountPushButton, SIGNAL(clicked(bool)), this, SLOT(resetReceiveDataCount()));
    connect(resetTxCountPushButton, SIGNAL(clicked(bool)), this, SLOT(resetSendDataCount()));

    /// 循环周期
    connect(cycleTimeLineEdit, SIGNAL(textChanged(QString)), this, SLOT(setCycleTime(QString)));

    /// 指示灯
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(updateRxImage()));
    connect(device, SIGNAL(bytesWritten(QByteArray)),  this, SLOT(updateTxImage()));

    /// 文本模式
    connect(inputModelComboBox, SIGNAL(currentTextChanged(QString)), this , SLOT(setInputMode(QString)));
    connect(outputModelComboBox, SIGNAL(currentTextChanged(QString)), this, SLOT(setOutputMode(QString)));

    /// 弹出自动回复设置面板
    connect(autoResponseSettingPushButton, SIGNAL(clicked(bool)), autoResponseSettingPanel, SLOT(show()));

    /// 自动回复
    connect(autoResponseSettingPanel, SIGNAL(autoResponseFlagChanged(bool)), this, SLOT(setAutoResponseFlag(bool)));
    connect(device, SIGNAL(bytesRead(QByteArray)), this, SLOT(handleReadBytes(QByteArray)));

    /// 高亮显示设置面板
    connect(highlighterSettingButton, &QPushButton::clicked, highlighterSettingPanel, &SAKHighlighterSettingPanel::show);

    /// 显示读写参数设置窗口、设置参数
    connect(rwParameterSettingButton, &QPushButton::clicked, &rwParameterSettingDialog, &SAKReadWriteSetting::show);
    connect(&rwParameterSettingDialog, &SAKReadWriteSetting::readDelayTimeChanged, device, &SAKIODevice::setReadDelayTime);
    connect(&rwParameterSettingDialog, &SAKReadWriteSetting::writeDelayTimeChanged, device, &SAKIODevice::setWriteDelayTime);
}

void SAKTabPage::afterDeviceOpen()
{
    controler->afterDeviceOpen();
    refreshPushButton->setEnabled(false);
    switchPushButton->setText(tr("关闭"));
}

void SAKTabPage::afterDeviceClose()
{
    controler->afterDeviceClose();
    refreshPushButton->setEnabled(true);
    switchPushButton->setText(tr("打开"));

    if (cycleTimer->isActive()){
        cycleTimer->stop();
        sendPushButton->setEnabled(true);
        cycleTimeLineEdit->setEnabled(true);
    }
}

void SAKTabPage::outputInfo(QString info, QString color, bool prefix)
{
    QString strTemp;
    if (prefix){
        strTemp += QDateTime::currentDateTime().toString("yyyy/MM/dd hh:mm:ss");
        strTemp = QString("<font color=silver>%1 </font>").arg(strTemp);
    }

    strTemp += QString("<font color=%1>%2</font>").arg(color).arg(info);
//    ui->labelOutput->setText(strTemp);
    clearInfoTimer->start();
}

void SAKTabPage::outputErrorString(QString str)
{
    outputInfo(str, "red");
}

void SAKTabPage::outputInformationString(QString str)
{
    outputInfo(str, "green");
}

void SAKTabPage::clearInfo()
{
//    ui->labelOutput->clear();
    if (clearInfoTimer->isActive()){
        clearInfoTimer->stop();
    }
}

void SAKTabPage::writeBytes()
{
    QString tipStr = tr("设备未就绪，本次发送操作取消！");

    if (cycleEnableCheckBox->isChecked()){
        if (device->isOpen()){
            qlonglong cycleTime = cycleTimeLineEdit->text().toLongLong();
            if (cycleTime == 0){
                cycleTime = 1000;
            }
            cycleTimer->start(static_cast<int>(cycleTime));
            sendPushButton->setEnabled(false);
            cycleTimeLineEdit->setEnabled(false);
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

void SAKTabPage::checkedBoxCycleClicked(bool checked)
{
    if (checked){

    }else{
        cycleTimer->stop();
        sendPushButton->setEnabled(true);
        cycleTimeLineEdit->setEnabled(true);
    }
}

void SAKTabPage::cycleTimerTimeout()
{
    if (device->isOpen()){
        emit need2writeBytes(dataBytes());
    }else {
        cycleTimer->stop();
        outputInfo(tr("设备未就绪，本次发送操作取消！"), "red");
    }
}

void SAKTabPage::cancleCycle()
{
    cycleEnableCheckBox->setChecked(false);
}

QByteArray SAKTabPage::dataBytes()
{
    QByteArray data;

    QString str = inputTextEdit->toPlainText();
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
        data = inputTextEdit->toPlainText().toLatin1();
    }else if (inputTextMode.compare(QString(textModel.valueToKey(Local8bit))) == 0){
        data = inputTextEdit->toPlainText().toLocal8Bit();
    }else {
        Q_ASSERT_X(false, __FUNCTION__, "Unknow input mode");
    }

    return data;
}

void SAKTabPage::textFormatControl()
{
    disconnect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
    QString plaintext = inputTextEdit->toPlainText();
    if (plaintext.isEmpty()){
        connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
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
            inputTextEdit->setText(strTemp);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Oct))) == 0) {
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
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Dec))) == 0) {
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
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Hex))) == 0) {
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
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Ascii))) == 0) {
            plaintext.remove(QRegExp("[^\0u00-\u007f ]"));
            inputTextEdit->setText(plaintext);
            inputTextEdit->moveCursor(QTextCursor::End);
        }else if(inputTextMode.compare(QString(textModel.valueToKey(Local8bit))) == 0) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow output mode");
        }
    }
    connect(inputTextEdit, SIGNAL(textChanged()), this, SLOT(textFormatControl()));
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

    refreshOutputData(data, true);
}

void SAKTabPage::bytesWritten(QByteArray data)
{
    qlonglong writeBytes = txBytesLabel->text().toLongLong();
    writeBytes += data.length();
    txBytesLabel->setText(QString::number(writeBytes));

    qlonglong writeFrame = txFramesLabel->text().toLongLong();
    writeFrame += 1;
    txFramesLabel->setText(QString::number(writeFrame));

    refreshOutputData(data, false);
}

void SAKTabPage::refreshOutputData(QByteArray &data, bool isReceivedData)
{
    Q_UNUSED(isReceivedData);
//    if (ui->checkBoxPause->isChecked()){
//        return;
//    }

    QString str;
//    if (!ui->checkBoxOutputReceiveDataOnly->isChecked()){
//        str.append("[");
//    }

    if (showDateCheckBox->isChecked()){
        str.append(QDate::currentDate().toString("yyyy/MM/dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (showTimeCheckBox->isChecked()){
        if (showMsCheckBox->isChecked()){
            str.append(QTime::currentTime().toString("hh:mm:ss.z "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

//    if (!ui->checkBoxOutputReceiveDataOnly->isChecked()){
//        if (isReceivedData){
//            str.append("<font color=blue>Rx</font>");
//        }else {
//            str.append("<font color=purple>Tx</font>");
//        }
//        str.append("<font color=silver>] </font>");
//    }

    outputTextMode = outputModelComboBox->currentText();
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

//    if (ui->checkBoxOutputReceiveDataOnly->isChecked()){
//        if (!isReceivedData){
//            return;
//        }

//        /**
//         * 以下这种追加文本的方式存在问题，在文本较多，追加频繁时，界面卡顿。
//         **/
//        if (!outputTextBroswer->toPlainText().isEmpty()){
//            str = outputTextBroswer->toPlainText() + " " + str;
//        }
//        outputTextBroswer->setText(QString("<font color=blue>%1</font>").arg(str));
//        outputTextBroswer->verticalScrollBar()->setSliderPosition(outputTextBroswer->verticalScrollBar()->maximum());
//    }else {
//        outputTextBroswer->append(str);
//    }
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
    QString option = QString("OutputMode");
    QString value = readSetting(option);

    /// 默认使用Ascii输入
    if (value.isEmpty()){
        value = QString(textModel.valueToKey(Ascii));
        setInputMode(value);
    }

    outputModelComboBox->setCurrentText(value);
}

void SAKTabPage::setInputMode(QString mode)
{
    QString option = QString("InputMode");
    writeSetting(option, mode);
    inputTextMode = mode;
}

void SAKTabPage::readInputMode()
{
    QString option = QString("InputMode");
    QString value = readSetting(option);

    /// 默认使用Ascii输入
    if (value.isEmpty()){
        value = QString(textModel.valueToKey(Ascii));
    }

    inputTextMode = value;
    textFormatControl();

    inputModelComboBox->setCurrentText(value);
}

void SAKTabPage::updateTxImage()
{
    static bool b = false;
    if (b){
        txLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }else{
        txLabel->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }

    b = !b;
}

void SAKTabPage::updateRxImage()
{
    static bool b = false;
    if (b){
        rxLabel->setPixmap(QPixmap(":/images/RtRxGray.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }else{
        rxLabel->setPixmap(QPixmap(":/images/RtRxRed.png").scaled(rxtxSize, Qt::KeepAspectRatio));
    }

    b = !b;
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

void SAKTabPage::setAutoResponseFlag(bool enableAutoResponse)
{
    QString option = QString("AutoResponse");
    QString value = enableAutoResponse ? QString("true") : QString("false");

    writeSetting(option, value);
}

void SAKTabPage::readAutoResponseFlag()
{
    QString option = QString("AutoResponse");
    QString value = readSetting(option);
    if (value.toUpper().compare(QString("TRUE")) == 0){
        autoResponseSettingPanel->setAutoResponseFlag(true);
    }else{
        autoResponseSettingPanel->setAutoResponseFlag(false);
    }
}

void SAKTabPage::handleReadBytes(QByteArray data)
{
    if (!autoResponseSettingPanel->autoResponseFlag()){
        return;
    }

    for (auto item:autoResponseSettingPanel->autoResponseItems()){
        if (item.receiveDataString.toUpper().compare(QString(data.toHex(' ')).toUpper()) == 0){
            QByteArray responseData;
            responseData.clear();
            QStringList tempList = item.sendDataString.split(' ');
            for (int i = 0; i < tempList.length(); i++){
                responseData.append(static_cast<char>(tempList.at(i).toInt(nullptr, 16)));
            }

            emit need2writeBytes(responseData);
            break;
        }
    }
}

QString SAKTabPage::readSetting(QString &option)
{
    QSettings settings;
    QString key = device->deviceName() + "/" + option;
    QString value = settings.value(key).toString();

    return value;
}

void SAKTabPage::writeSetting(QString &option, QString &value)
{
    QSettings settings;
    QString key = device->deviceName() + "/" + option;

    settings.setValue(key, value);
}

void SAKTabPage::initUiPointer()
{
    /*
     * 设备管理组
     */
    //    refreshPushButton   = ui->refreshPushButton;
    //    switchPushButton    = ui->switchPushButton;
    //    rxLabel             = ui->rxLabel;
    //    txLabel             = ui->txLabel;

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

    /*
     * 输出设置组
     */
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

void SAKTabPage::setLabelText(QLabel *label, quint64 text)
{
    label->setText(QString::number(text));
}

void SAKTabPage::on_outputModelComboBox_currentTextChanged(const QString &text)
{
    /// 在ui初始化的时候，会出现text为empty的情况
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

void SAKTabPage::on_showDateCheckBox_clicked()
{
    if (showDateCheckBox->isChecked()) {
        showDate = true;
    }else{
        showDate = false;
    }
}

void SAKTabPage::on_autoWrapCheckBox_clicked()
{
    if (autoWrapCheckBox->isChecked()){
        autoWrap = true;
    }else{
        autoWrap = false;
    }
}

void SAKTabPage::on_showTimeCheckBox_clicked()
{
    if (showTimeCheckBox->isChecked()){
        showTime = true;
    }else{
        showTime = false;
    }
}

void SAKTabPage::on_showMsCheckBox_clicked()
{
    if (showMsCheckBox->isChecked()){
        showMS = true;
    }else{
        showMS = false;
    }
}

void SAKTabPage::on_showRxDataCheckBox_clicked()
{
    if (showRxDataCheckBox->isChecked()){
        showRxData = true;
    }else{
        showRxData = false;
    }
}

void SAKTabPage::on_showTxDataCheckBox_clicked()
{
    if (showTxDataCheckBox->isChecked()){
        showTxData = true;
    }else {
        showTxData = false;
    }
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
    /// 在ui初始化的时候，会出现text为empty的情况
    if (text.isEmpty()){
        return;
    }

    QMetaEnum model = QMetaEnum::fromType<TextDisplayModel>();
    bool ok = false;
    int ret = model.keyToValue(text.toLatin1().data(), &ok);
    if (ok){
        inputTextMode = static_cast<TextDisplayModel>(ret);
    }else{
        QLoggingCategory category(logCategory);
        qCWarning(category) << "Input text model error!";
    }
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
    emit need2write();
}
