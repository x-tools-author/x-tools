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
 */
#include "SAKGlobal.hh"
#include "OutputDataFactory.hh"
#include "DebugPageOutputManager.hh"
#include "SaveOutputDataSettings.hh"

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QFileDialog>

DebugPageOutputManager::DebugPageOutputManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
    ,rxFlagCount (4)
    ,txFlagCount (0)
{
    // 注册数据类型（自定义数据类型作为信号参数时，要进行注册，该函数要与宏 Q_DECLARE_METATYPE 配套使用）
    qRegisterMetaType<OutputParameters>("OutputParameters");

    rxLabel                         = debugPage->rxLabel;
    txLabel                         = debugPage->txLabel;
    txTextFormatComboBox            = debugPage->txTextFormatComboBox;
    rxTextFormatComboBox            = debugPage->rxTextFormatComboBox;
    showDateCheckBox                = debugPage->showDateCheckBox;
    autoWrapCheckBox                = debugPage->autoWrapCheckBox;
    showTimeCheckBox                = debugPage->showTimeCheckBox;
    showMsCheckBox                  = debugPage->showMsCheckBox;
    showRxDataCheckBox              = debugPage->showRxDataCheckBox;
    showTxDataCheckBox              = debugPage->showTxDataCheckBox;
    saveOutputFileToFilecheckBox    = debugPage->saveOutputFileToFilecheckBox;
    outputFilePathPushButton        = debugPage->outputFilePathPushButton;
    clearOutputPushButton           = debugPage->clearOutputPushButton;
    saveOutputPushButton            = debugPage->saveOutputPushButton;
    outputTextBroswer               = debugPage->outputTextBroswer;

    connect(saveOutputFileToFilecheckBox,   &QCheckBox::clicked, this, &DebugPageOutputManager::saveOutputDataToFile);
    connect(autoWrapCheckBox,               &QCheckBox::clicked, this, &DebugPageOutputManager::setLineWrapMode);
    connect(saveOutputPushButton,           &QCheckBox::clicked, this, &DebugPageOutputManager::saveOutputTextToFile);
    connect(outputFilePathPushButton,       &QCheckBox::clicked, this, &DebugPageOutputManager::saveOutputDataSettings);


    // 初始化数据格式预选框
    SAKGlobal::initOutputTextFormatComboBox(txTextFormatComboBox);
    SAKGlobal::initOutputTextFormatComboBox(rxTextFormatComboBox);

    /*
     * 处理已接收或者是已发送的数据
     */
    connect(debugPage, &SAKDebugPage::bytesRead, this, &DebugPageOutputManager::bytesRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &DebugPageOutputManager::bytesWritten);

    /*
     * 数据先发送到 dataFactory 中进行处理，处理完毕后再输出至界面
     */
    dataFactory = new OutputDataFactory;
    connect(this, &DebugPageOutputManager::cookData, dataFactory, &OutputDataFactory::cookData);
    connect(dataFactory, &OutputDataFactory::dataCooked, this, &DebugPageOutputManager::outputData);
    dataFactory->start();

    outputSettings = new SaveOutputDataSettings;

    updateRxFlagTimer.setInterval(200);
    updateTxFlagTimer.setInterval(200);
    connect(&updateRxFlagTimer, &QTimer::timeout, this, &DebugPageOutputManager::updateRxFlag);
    connect(&updateTxFlagTimer, &QTimer::timeout, this, &DebugPageOutputManager::updateTxFlag);

    outputTextBroswer->document()->setMaximumBlockCount(1000);
}

DebugPageOutputManager::~DebugPageOutputManager()
{
    dataFactory->terminate();
    delete dataFactory;
    delete outputSettings;
}

void DebugPageOutputManager::updateRxFlag()
{
    updateRxFlagTimer.stop();
    rxLabel->setText(QString("C%1").arg(QString(""), rxFlagCount, '<'));

    rxFlagCount -= 1;
    if (rxFlagCount == -1){
        rxFlagCount = 8;
    }
}

void DebugPageOutputManager::updateTxFlag()
{
    updateTxFlagTimer.stop();
    txLabel->setText(QString("C%1").arg(QString(""), txFlagCount, '>'));

    txFlagCount += 1;
    if (txFlagCount == 8){
        txFlagCount = 0;
    }
}

void DebugPageOutputManager::setLineWrapMode()
{
    if (autoWrapCheckBox->isChecked()){
        outputTextBroswer->setLineWrapMode(QTextEdit::WidgetWidth);
    }else{
        outputTextBroswer->setLineWrapMode(QTextEdit::NoWrap);
    }
}

void DebugPageOutputManager::saveOutputTextToFile()
{
    QString outFileName = QFileDialog::getSaveFileName(nullptr,
                                                       tr("保存文件"),
                                                       QString("./%1.txt")
                                                       .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")),
                                                       tr("文本 (*.txt)"));
    if (outFileName.isEmpty()){
        return;
    }

    QFile outFile(outFileName);
    if(outFile.open(QIODevice::WriteOnly|QIODevice::Text)){
        QTextStream outStream(&outFile);
        outStream << outputTextBroswer->toPlainText();
        outFile.flush();
        outFile.close();
    }else{
        debugPage->outputMessage(QString("Can not open file (%1) to save output data:")
                                 .arg(outFile.fileName()) + outFile.errorString(), false);
    }
}

void DebugPageOutputManager::saveOutputDataSettings()
{
    outputSettings->show();
}

void DebugPageOutputManager::saveOutputDataToFile()
{
    if (saveOutputFileToFilecheckBox->isChecked()){
        connect(debugPage, &SAKDebugPage::bytesRead, outputSettings, &SaveOutputDataSettings::inputData);
    }else{
        disconnect(debugPage, &SAKDebugPage::bytesRead, outputSettings, &SaveOutputDataSettings::inputData);
    }
}

void DebugPageOutputManager::bytesRead(QByteArray data)
{
    if (!updateRxFlagTimer.isActive()){
        updateRxFlagTimer.start();
    }

    if (!showRxDataCheckBox->isChecked()){
        return;
    }

    OutputParameters parameters = outputDataParameters(true);
    emit cookData(data, parameters);
}

void DebugPageOutputManager::bytesWritten(QByteArray data)
{
    if (!updateTxFlagTimer.isActive()){
        updateTxFlagTimer.start();
    }

    if (!showTxDataCheckBox->isChecked()){
        return;
    }

    OutputParameters parameters = outputDataParameters(false);
    emit cookData(data, parameters);
}

void DebugPageOutputManager::outputData(QString data)
{
    outputTextBroswer->append(data);
}

DebugPageOutputManager::OutputParameters DebugPageOutputManager::outputDataParameters(bool isReceivedData)
{
    OutputParameters parameters;
    parameters.showDate = showDateCheckBox->isChecked();
    parameters.showTime = showTimeCheckBox->isChecked();
    parameters.showMS   = showMsCheckBox->isChecked();
    parameters.isReceivedData = isReceivedData;

    if (isReceivedData){
        parameters.textModel= rxTextFormatComboBox->currentData().toInt();
    }else{
        parameters.textModel= txTextFormatComboBox->currentData().toInt();
    }

    return parameters;
}
