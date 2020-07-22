/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QFile>
#include <QDateTime>
#include <QTextStream>
#include <QFileDialog>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKOutputDataFactory.hh"
#include "SAKSaveOutputDataSettings.hh"
#include "SAKDebugPageOutputController.hh"

SAKDebugPageOutputController::SAKDebugPageOutputController(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,mDebugPage (debugPage)
    ,mRxAnimationgCount (5)
    ,mTxAnimationCount (0)
{
    // OutputParameters will be signal parameter, the step must be execute
    qRegisterMetaType<OutputParameters>("OutputParameters");

    // initialize ui component
    mRxLabel = debugPage->mRxLabel;
    mTxLabel = debugPage->mTxLabel;
    mOutputTextFormatComboBox = debugPage->mOutputTextFormatComboBox;
    mShowDateCheckBox = debugPage->mShowDateCheckBox;
    mAutoWrapCheckBox = debugPage->mAutoWrapCheckBox;
    mShowTimeCheckBox = debugPage->mShowTimeCheckBox;
    mShowMsCheckBox = debugPage->mShowMsCheckBox;
    mShowRxDataCheckBox = debugPage->mShowRxDataCheckBox;
    mShowTxDataCheckBox = debugPage->mShowTxDataCheckBox;
    mSaveOutputFileToFilecheckBox = debugPage->mSaveOutputFileToFilecheckBox;
    mOutputFilePathPushButton = debugPage->mOutputFilePathPushButton;
    mClearOutputPushButton = debugPage->mClearOutputPushButton;
    mSaveOutputPushButton = debugPage->mSaveOutputPushButton;
    mOutputTextBroswer = debugPage->mOutputTextBroswer;
    SAKGlobal::initOutputTextFormatComboBox(mOutputTextFormatComboBox);

    connect(mSaveOutputFileToFilecheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::saveOutputDataToFile);
    connect(mAutoWrapCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::setLineWrapMode);
    connect(mSaveOutputPushButton, &QCheckBox::clicked, this, &SAKDebugPageOutputController::saveOutputTextToFile);
    connect(mOutputFilePathPushButton, &QCheckBox::clicked, this, &SAKDebugPageOutputController::saveOutputDataSettings);

    // input data
    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKDebugPageOutputController::bytesRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKDebugPageOutputController::bytesWritten);

    // handle raw data
    mDataFactory = new SAKOutputDataFactory;
    connect(this, &SAKDebugPageOutputController::cookData, mDataFactory, &SAKOutputDataFactory::cookData);
    connect(mDataFactory, &SAKOutputDataFactory::dataCooked, this, &SAKDebugPageOutputController::outputData);
    mDataFactory->start();

    // animation
    mUpdateRxAnimationTimer.setInterval(20);
    mUpdateTxAnimationTimer.setInterval(20);
    connect(&mUpdateRxAnimationTimer, &QTimer::timeout, this, &SAKDebugPageOutputController::updateRxAnimation);
    connect(&mUpdateTxAnimationTimer, &QTimer::timeout, this, &SAKDebugPageOutputController::updateTxAnimation);

    // do something make memory happy
    mOutputTextBroswer->document()->setMaximumBlockCount(1000);

    // the class is used to save data to file
    mOutputSettings = new SAKSaveOutputDataSettings;
}

SAKDebugPageOutputController::~SAKDebugPageOutputController()
{
    delete mDataFactory;
    delete mOutputSettings;
}

void SAKDebugPageOutputController::updateRxAnimation()
{
    mUpdateRxAnimationTimer.stop();
    mRxLabel->setText(QString("C%1").arg(QString(""), mRxAnimationgCount, '<'));

    mRxAnimationgCount -= 1;
    if (mRxAnimationgCount == -1){
        mRxAnimationgCount = 5;
    }
}

void SAKDebugPageOutputController::updateTxAnimation()
{
    mUpdateTxAnimationTimer.stop();
    mTxLabel->setText(QString("C%1").arg(QString(""), mTxAnimationCount, '>'));

    mTxAnimationCount += 1;
    if (mTxAnimationCount == 6){
        mTxAnimationCount = 0;
    }
}

void SAKDebugPageOutputController::setLineWrapMode()
{
    if (mAutoWrapCheckBox->isChecked()){
        mOutputTextBroswer->setLineWrapMode(QTextEdit::WidgetWidth);
    }else{
        mOutputTextBroswer->setLineWrapMode(QTextEdit::NoWrap);
    }
}

void SAKDebugPageOutputController::saveOutputTextToFile()
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
        outStream << mOutputTextBroswer->toPlainText();
        outFile.flush();
        outFile.close();
    }else{
        mDebugPage->outputMessage(QString("Can not open file (%1) to save output data:")
                                 .arg(outFile.fileName()) + outFile.errorString(), false);
    }
}

void SAKDebugPageOutputController::saveOutputDataSettings()
{
    mOutputSettings->show();
}

void SAKDebugPageOutputController::saveOutputDataToFile()
{
    if (mSaveOutputFileToFilecheckBox->isChecked()){
        connect(mDebugPage, &SAKDebugPage::bytesRead, mOutputSettings, &SAKSaveOutputDataSettings::inputData);
    }else{
        disconnect(mDebugPage, &SAKDebugPage::bytesRead, mOutputSettings, &SAKSaveOutputDataSettings::inputData);
    }
}

void SAKDebugPageOutputController::bytesRead(QByteArray data)
{
    if (!mUpdateRxAnimationTimer.isActive()){
        mUpdateRxAnimationTimer.start();
    }

    if (!mShowRxDataCheckBox->isChecked()){
        return;
    }

    OutputParameters parameters = outputDataParameters(true);
    emit cookData(data, parameters);
}

void SAKDebugPageOutputController::bytesWritten(QByteArray data)
{
    if (!mUpdateTxAnimationTimer.isActive()){
        mUpdateTxAnimationTimer.start();
    }

    if (!mShowTxDataCheckBox->isChecked()){
        return;
    }

    OutputParameters parameters = outputDataParameters(false);
    emit cookData(data, parameters);
}

void SAKDebugPageOutputController::outputData(QString data)
{
    mOutputTextBroswer->append(data);
}

SAKDebugPageOutputController::OutputParameters SAKDebugPageOutputController::outputDataParameters(bool isReceivedData)
{
    OutputParameters parameters;
    parameters.showDate = mShowDateCheckBox->isChecked();
    parameters.showTime = mShowTimeCheckBox->isChecked();
    parameters.showMS = mShowMsCheckBox->isChecked();
    parameters.isReadData = isReceivedData;

    parameters.format= mOutputTextFormatComboBox->currentData().toInt();

    return parameters;
}
