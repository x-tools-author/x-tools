/*
 * Copyright (C) 2020 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QFile>
#include <QDebug>
#include <QTextStream>
#include <QFileDialog>
#include <QStandardPaths>

#include "SAKDebugger.hh"
#include "SAKDebuggerDevice.hh"
#include "SAKDebuggerTextOutput.hh"
#include "SAKDebuggerOutputManager.hh"
#include "SAKDebuggerOutputSettings.hh"
#include "SAKDebuggerOutputTextFactory.hh"

SAKDebuggerTextOutput::SAKDebuggerTextOutput(SAKDebugger *debugger, QObject *parent)
    :QObject (parent)
    ,debugger (debugger)
{
    textFactory = new SAKDebuggerOutputTextFactory(debugger);
    connect(textFactory, &SAKDebuggerOutputTextFactory::dataCooked, this, &SAKDebuggerTextOutput::dataCooked);
}

SAKDebuggerTextOutput::~SAKDebuggerTextOutput()
{
    if (textFactory){
        textFactory->requestInterruption();
        textFactory->quit();
        textFactory->wait();
        delete textFactory;
    }
}

SAKDebuggerOutputTextFactory *SAKDebuggerTextOutput::textFactoryInstance()
{
    return textFactory;
}

void SAKDebuggerTextOutput::setDevice(SAKDebuggerDevice *device)
{
    if (device){
        connect(device, &SAKDebuggerDevice::bytesRead, this, &SAKDebuggerTextOutput::outputTextRx, Qt::QueuedConnection);
        connect(device, &SAKDebuggerDevice::bytesWritten, this, &SAKDebuggerTextOutput::outputTextTx, Qt::QueuedConnection);
        outputSettings = debugger->outputManagerInstance()->outputSettingsInstance();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Oh, a null pointer!");
    }
}

void SAKDebuggerTextOutput::saveDataToFile(QString data)
{
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR, tr("保存文件"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+"/SAKData.txt",
                                                    tr("Text (*.txt);;All (*)"));
    if (fileName.length()){
        QFile file(fileName);
        if (file.open(QFile::WriteOnly|QFile::Truncate|QFile::Text)){
            QTextStream outStream(&file);
                outStream << data;
        }
    }
    qDebug() << data;
}

void SAKDebuggerTextOutput::outputText(QByteArray text, bool isRxData)
{
    textFactory->addRawData(text, isRxData);
    /// @brief 唤醒处理线程处理数据
    textFactory->wakeMe();
}

void SAKDebuggerTextOutput::outputTextRx(QByteArray text)
{
    outputText(text, true);
}

void SAKDebuggerTextOutput::outputTextTx(QByteArray text)
{
    outputText(text, false);
}

void SAKDebuggerTextOutput::dataCooked(QString text)
{
    emit outputTextRequest(text);
}
