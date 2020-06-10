/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKGlobal.hh"
#include "SAKOutputDataFactory.hh"
#include "SAKDebugPageOutputManager.hh"
#include "SAKSaveOutputDataSettings.hh"

#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QFileDialog>

SAKDebugPageOutputManager::SAKDebugPageOutputManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
    ,rxFlagCount (4)
    ,txFlagCount (0)
{
    /// @brief 注册数据类型（自定义数据类型作为信号参数时，要进行注册，该函数要与宏 Q_DECLARE_METATYPE 配套使用）
    qRegisterMetaType<OutputParameters>("OutputParameters");

    rxLabel                         = debugPage->rxLabel;
    txLabel                         = debugPage->txLabel;
    outputTextFormatComboBox        = debugPage->outputTextFormatComboBox;
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

    connect(saveOutputFileToFilecheckBox,   &QCheckBox::clicked, this, &SAKDebugPageOutputManager::saveOutputDataToFile);
    connect(autoWrapCheckBox,               &QCheckBox::clicked, this, &SAKDebugPageOutputManager::setLineWrapMode);
    connect(saveOutputPushButton,           &QCheckBox::clicked, this, &SAKDebugPageOutputManager::saveOutputTextToFile);
    connect(outputFilePathPushButton,       &QCheckBox::clicked, this, &SAKDebugPageOutputManager::saveOutputDataSettings);

    /// @brief 初始化数据格式预选框
    SAKGlobal::initOutputTextFormatComboBox(outputTextFormatComboBox);

    /// @brief 处理已接收或者是已发送的数据
    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKDebugPageOutputManager::bytesRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKDebugPageOutputManager::bytesWritten);

    /// @brief 数据先发送到 dataFactory 中进行处理，处理完毕后再输出至界面
    dataFactory = new SAKOutputDataFactory;
    connect(this, &SAKDebugPageOutputManager::cookData, dataFactory, &SAKOutputDataFactory::cookData);
    connect(dataFactory, &SAKOutputDataFactory::dataCooked, this, &SAKDebugPageOutputManager::outputData);
    dataFactory->start();

    outputSettings = new SAKSaveOutputDataSettings;

    updateRxFlagTimer.setInterval(20);
    updateTxFlagTimer.setInterval(20);
    connect(&updateRxFlagTimer, &QTimer::timeout, this, &SAKDebugPageOutputManager::updateRxFlag);
    connect(&updateTxFlagTimer, &QTimer::timeout, this, &SAKDebugPageOutputManager::updateTxFlag);

    outputTextBroswer->document()->setMaximumBlockCount(1000);
}

SAKDebugPageOutputManager::~SAKDebugPageOutputManager()
{
    delete dataFactory;
    delete outputSettings;
}

void SAKDebugPageOutputManager::updateRxFlag()
{
    updateRxFlagTimer.stop();
    rxLabel->setText(QString("C%1").arg(QString(""), rxFlagCount, '<'));

    rxFlagCount -= 1;
    if (rxFlagCount == -1){
        rxFlagCount = 8;
    }
}

void SAKDebugPageOutputManager::updateTxFlag()
{
    updateTxFlagTimer.stop();
    txLabel->setText(QString("C%1").arg(QString(""), txFlagCount, '>'));

    txFlagCount += 1;
    if (txFlagCount == 8){
        txFlagCount = 0;
    }
}

void SAKDebugPageOutputManager::setLineWrapMode()
{
    if (autoWrapCheckBox->isChecked()){
        outputTextBroswer->setLineWrapMode(QTextEdit::WidgetWidth);
    }else{
        outputTextBroswer->setLineWrapMode(QTextEdit::NoWrap);
    }
}

void SAKDebugPageOutputManager::saveOutputTextToFile()
{
    QString outFileName = QFileDialog::getSaveFileName(Q_NULLPTR,
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

void SAKDebugPageOutputManager::saveOutputDataSettings()
{
    outputSettings->show();
}

void SAKDebugPageOutputManager::saveOutputDataToFile()
{
    if (saveOutputFileToFilecheckBox->isChecked()){
        connect(debugPage, &SAKDebugPage::bytesRead, outputSettings, &SAKSaveOutputDataSettings::inputData);
    }else{
        disconnect(debugPage, &SAKDebugPage::bytesRead, outputSettings, &SAKSaveOutputDataSettings::inputData);
    }
}

void SAKDebugPageOutputManager::bytesRead(QByteArray data)
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

void SAKDebugPageOutputManager::bytesWritten(QByteArray data)
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

void SAKDebugPageOutputManager::outputData(QString data)
{
    outputTextBroswer->append(data);
}

SAKDebugPageOutputManager::OutputParameters SAKDebugPageOutputManager::outputDataParameters(bool isReceivedData)
{
    OutputParameters parameters;
    parameters.showDate = showDateCheckBox->isChecked();
    parameters.showTime = showTimeCheckBox->isChecked();
    parameters.showMS   = showMsCheckBox->isChecked();
    parameters.isReceivedData = isReceivedData;

    parameters.textModel= outputTextFormatComboBox->currentData().toInt();

    return parameters;
}
