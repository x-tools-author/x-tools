/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMenu>
#include <QFile>
#include <QDebug>
#include <QDateTime>
#include <QTextStream>
#include <QFileDialog>

#include "SAKDebugPage.hh"
#include "SAKOutputLogDialog.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKOutputSave2FileDialog.hh"
#include "SAKOtherHighlighterManager.hh"
#include "SAKDebugPageOutputController.hh"

SAKDebugPageOutputController::SAKDebugPageOutputController(SAKDebugPage *debugPage, QObject *parent)
    :QThread(parent)
    ,mDebugPage(debugPage)
    ,mSettings(Q_NULLPTR)
    ,mSave2FileDialog(Q_NULLPTR)
    ,mHasBeenClear(false)
    ,mRxAnimationgCount(5)
    ,mTxAnimationCount(0)
{
    // Initialize ui component
    mRxLabel = debugPage->mRxLabel;
    mTxLabel = debugPage->mTxLabel;
    mOutputTextFormatComboBox = debugPage->mOutputTextFormatComboBox;
    mShowDateCheckBox = debugPage->mShowDateCheckBox;
    mAutoWrapCheckBox = debugPage->mAutoWrapCheckBox;
    mShowTimeCheckBox = debugPage->mShowTimeCheckBox;
    mShowMsCheckBox = debugPage->mShowMsCheckBox;
    mShowRxDataCheckBox = debugPage->mShowRxDataCheckBox;
    mShowTxDataCheckBox = debugPage->mShowTxDataCheckBox;
    mSaveOutputToFileCheckBox = debugPage->mSaveOutputToFileCheckBox;
    mRawDataCheckBox = debugPage->mRawDataCheckBox;
    mMoreOutputSettingsPushButton = debugPage->mMoreOutputSettingsPushButton;
    mClearOutputPushButton = debugPage->mClearOutputPushButton;
    mOutputTextBroswer = debugPage->mOutputTextBroswer;
    SAKCommonDataStructure::setComboBoxTextOutputFormat(mOutputTextFormatComboBox);

    // Initializing setting keys
    QString group = mDebugPage->settingsGroup();
    mSettingStringOutputTextFormat = QString("%1/outputTextFormat").arg(group);
    mSettingStringShowDate = QString("%1/showDate").arg(group);
    mSettingStringAutoWrap = QString("%1/autoWrap").arg(group);
    mSettingStringShowTime = QString("%1/showTime").arg(group);
    mSettingStringShowMs = QString("%1/showMs").arg(group);
    mSettingStringShowRx = QString("%1/showRx").arg(group);
    mSettingStringShowTx = QString("%1/showTx").arg(group);
    mSettingStringRawData = QString("%1/rawData").arg(group);

    // Readin settings before connecting signals and slots
    mSettings = mDebugPage->settings();
    readinSettings();

    // Connecting signals and slots
    connect(mSaveOutputToFileCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::saveOutputDataToFile);
    connect(mAutoWrapCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::setLineWrapMode);
    connect(mClearOutputPushButton, &QCheckBox::clicked, mOutputTextBroswer, &QTextBrowser::clear);
    connect(mOutputTextFormatComboBox, &QComboBox::currentTextChanged, this, &SAKDebugPageOutputController::onOutputTextFormatComboBoxCurrentTextChanged);
    connect(mShowDateCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onShowDateCheckBoxClicked);
    connect(mAutoWrapCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onAutoWrapCheckBoxClicked);
    connect(mShowTimeCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onShowTimeCheckBoxClicked);
    connect(mShowMsCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onShowMsCheckBoxClicked);
    connect(mShowRxDataCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onShowRxDataCheckBoxClicked);
    connect(mShowTxDataCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onShowTxDataCheckBoxClicked);
    connect(mRawDataCheckBox, &QCheckBox::clicked, this, &SAKDebugPageOutputController::onRawDataCheckBoxClicked);

    // Input data
    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKDebugPageOutputController::bytesRead);
    connect(debugPage, &SAKDebugPage::bytesWritten, this, &SAKDebugPageOutputController::bytesWritten);

    // Output data
    connect(this, &SAKDebugPageOutputController::dataCooked, this, &SAKDebugPageOutputController::outputData);

    // Animation
    mUpdateRxAnimationTimer.setInterval(20);
    mUpdateTxAnimationTimer.setInterval(20);
    connect(&mUpdateRxAnimationTimer, &QTimer::timeout, this, &SAKDebugPageOutputController::updateRxAnimation);
    connect(&mUpdateTxAnimationTimer, &QTimer::timeout, this, &SAKDebugPageOutputController::updateTxAnimation);

    // Do something make memory happy
    mOutputTextBroswer->document()->setMaximumBlockCount(1000);

    // The class is used to save data to file
    mSave2FileDialog = new SAKOutputSave2FileDialog(mDebugPage);
    mSAKOutputLogDialog = new SAKOutputLogDialog(mDebugPage);
    mSAKOtherHighlighterManager = new SAKOtherHighlighterManager(mOutputTextBroswer->document());

    // More output settings menu
    auto moreOutputSettingsPushButtonMenu = new QMenu;
    mMoreOutputSettingsPushButton->setMenu(moreOutputSettingsPushButtonMenu);
    QAction *saveAction = new QAction(tr("Save to File"), this);
    moreOutputSettingsPushButtonMenu->addAction(saveAction);
    connect(saveAction, &QAction::triggered, this, &SAKDebugPageOutputController::saveOutputTextToFile);
    QAction *saveToFileAction = new QAction(tr("Write to File"), this);
    moreOutputSettingsPushButtonMenu->addAction(saveToFileAction);
    connect(saveToFileAction, &QAction::triggered, mSave2FileDialog, &SAKOutputSave2FileDialog::show);
    QAction *logAction = new QAction(tr("Log Output View"), this);
    moreOutputSettingsPushButtonMenu->addAction(logAction);
    connect(logAction, &QAction::triggered, mSAKOutputLogDialog, &SAKOutputLogDialog::show);
    QAction *highlightSettingsAction = new QAction(tr("Highlight Settings"), this);
    moreOutputSettingsPushButtonMenu->addAction(highlightSettingsAction);
    connect(highlightSettingsAction, &QAction::triggered, mSAKOtherHighlighterManager, &SAKOtherHighlighterManager::show);

    // The thread will started when the class is initailzed
    start();
}

SAKDebugPageOutputController::~SAKDebugPageOutputController()
{
    // Exit the thread first
    requestInterruption();
    mThreadWaitCondition.wakeAll();
    exit();
    wait();

    // Free memory
    delete mSave2FileDialog;
}

void SAKDebugPageOutputController::outputLog(QString log, bool isInfo)
{
    mSAKOutputLogDialog->outputMessage(log, isInfo);
}

void SAKDebugPageOutputController::run()
{
    QEventLoop eventLoop;
    while (true) {
        // Cook data
        while (true) {
            RawDataStruct rawData = takeRawData();
            if (rawData.rawData.length()){
                innerCookData(rawData.rawData, rawData.parameters);
            }else{
                break;
            }
        }

        // Do something make thread inner happy
        eventLoop.processEvents();

        // If is interruption requested, the thread will exit, or the thread will sleep
        if (isInterruptionRequested()){
            break;
        }else{
            mThreadMutex.lock();
            mThreadWaitCondition.wait(&mThreadMutex, 50);
            mThreadMutex.unlock();
        }
    }
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
                                                       tr("Save to file"),
                                                       QString("./%1.txt")
                                                       .arg(QDateTime::currentDateTime().toString("yyyyMMddhhmmss")),
                                                       QString("txt (*.txt)"));
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
    mSave2FileDialog->show();
}

void SAKDebugPageOutputController::saveOutputDataToFile()
{
    if (mSaveOutputToFileCheckBox->isChecked()){
        connect(mDebugPage, &SAKDebugPage::bytesRead, mSave2FileDialog, &SAKOutputSave2FileDialog::bytesRead);
        connect(mDebugPage, &SAKDebugPage::bytesWritten, mSave2FileDialog, &SAKOutputSave2FileDialog::bytesWritten);
    }else{
        disconnect(mDebugPage, &SAKDebugPage::bytesRead, mSave2FileDialog, &SAKOutputSave2FileDialog::bytesRead);
        disconnect(mDebugPage, &SAKDebugPage::bytesWritten, mSave2FileDialog, &SAKOutputSave2FileDialog::bytesWritten);
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

    RawDataStruct rawData;
    OutputParameters parameters = outputDataParameters(true);
    rawData.rawData = data;
    rawData.parameters = parameters;
    mRawDataListMutex.lock();
    mRawDataList.append(rawData);
    mRawDataListMutex.unlock();

    // Wake the thead to handle raw data
    mThreadWaitCondition.wakeAll();
}

void SAKDebugPageOutputController::bytesWritten(QByteArray data)
{
    if (!mUpdateTxAnimationTimer.isActive()){
        mUpdateTxAnimationTimer.start();
    }

    if (!mShowTxDataCheckBox->isChecked()){
        return;
    }

    RawDataStruct rawData;
    OutputParameters parameters = outputDataParameters(false);
    rawData.rawData = data;
    rawData.parameters = parameters;
    mRawDataListMutex.lock();
    mRawDataList.append(rawData);
    mRawDataListMutex.unlock();

    // Wake the thead to handle raw data
    mThreadWaitCondition.wakeAll();
}

void SAKDebugPageOutputController::outputData(QString data, bool rawData)
{
    if (!mHasBeenClear){
        mHasBeenClear = true;
        mOutputTextBroswer->clear();
    }

    if (rawData){
        mOutputTextBroswer->textCursor().insertText(data);
    }else{
        mOutputTextBroswer->append(data);
    }
}

SAKDebugPageOutputController::OutputParameters SAKDebugPageOutputController::outputDataParameters(bool isReceivedData)
{
    OutputParameters parameters;
    parameters.showDate = mShowDateCheckBox->isChecked();
    parameters.showTime = mShowTimeCheckBox->isChecked();
    parameters.showMS = mShowMsCheckBox->isChecked();
    parameters.isReadData = isReceivedData;
    parameters.isRawData = mRawDataCheckBox->isChecked();
    parameters.format= mOutputTextFormatComboBox->currentData().toInt();

    return parameters;
}

SAKDebugPageOutputController::RawDataStruct SAKDebugPageOutputController::takeRawData()
{
    RawDataStruct rawData;
    mRawDataListMutex.lock();
    if (mRawDataList.length()){
        rawData = mRawDataList.takeFirst();
    }
    mRawDataListMutex.unlock();

    return rawData;
}

void SAKDebugPageOutputController::readinSettings()
{
    auto setValue = [](QVariant &var){
        if (var.isNull()){
            return true;
        }else{
            return var.toBool();
        }
    };

    QVariant var = mSettings->value(mSettingStringOutputTextFormat);
    int index = SAKCommonDataStructure::OutputFormatHex;
    if (!var.isNull()){
        index = var.toInt();;
    }
    mOutputTextFormatComboBox->setCurrentIndex(index);

    var = mSettings->value(mSettingStringShowDate);
    bool value = mSettings->value(mSettingStringShowDate).toBool();
    mShowDateCheckBox->setChecked(value);

    var = mSettings->value(mSettingStringAutoWrap);
    value = setValue(var);
    mAutoWrapCheckBox->setChecked(value);

    value = mSettings->value(mSettingStringShowTime).toBool();
    mShowTimeCheckBox->setChecked(value);

    value = mSettings->value(mSettingStringShowMs).toBool();
    mShowMsCheckBox->setChecked(value);

    var = mSettings->value(mSettingStringShowRx);
    value = setValue(var);
    mShowRxDataCheckBox->setChecked(value);

    var = mSettings->value(mSettingStringShowTx);
    value = setValue(var);
    mShowTxDataCheckBox->setChecked(value);

    var = mSettings->value(mSettingStringRawData);
    value = var.isNull() ? false : var.toBool();
    mRawDataCheckBox->setChecked(value);
}

void SAKDebugPageOutputController::innerCookData(QByteArray rawData, OutputParameters parameters)
{
    QString str;
    str.append("<font color=silver>[</font>");

    if (parameters.showDate){
        str.append(QDate::currentDate().toString("yyyy-MM-dd "));
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.showTime){
        if (parameters.showMS){
            str.append(QTime::currentTime().toString("hh:mm:ss.zzz "));
        }else {
            str.append(QTime::currentTime().toString("hh:mm:ss "));
        }
        str = QString("<font color=silver>%1</font>").arg(str);
    }

    if (parameters.isReadData){
        str.append("<font color=red>Rx</font>");
    }else {
        str.append("<font color=blue>Tx</font>");
    }
    str.append("<font color=silver>] </font>");

    auto dataString = SAKCommonDataStructure::byteArrayToString(rawData, static_cast<SAKCommonDataStructure::SAKEnumTextOutputFormat>(parameters.format));
    if (parameters.isRawData){
        str = dataString;
    }else{
        str.append(dataString);
    }
    emit dataCooked(str, parameters.isRawData);
}

void SAKDebugPageOutputController::onOutputTextFormatComboBoxCurrentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    mSettings->setValue(mSettingStringOutputTextFormat, QVariant::fromValue(mOutputTextFormatComboBox->currentIndex()));
}

void SAKDebugPageOutputController::onShowDateCheckBoxClicked()
{
    mSettings->setValue(mSettingStringShowDate, QVariant::fromValue(mShowDateCheckBox->isChecked()));
}

void SAKDebugPageOutputController::onAutoWrapCheckBoxClicked()
{
    mSettings->setValue(mSettingStringAutoWrap, QVariant::fromValue(mAutoWrapCheckBox->isChecked()));
}

void SAKDebugPageOutputController::onShowTimeCheckBoxClicked()
{
    mSettings->setValue(mSettingStringShowTime, QVariant::fromValue(mShowTimeCheckBox->isChecked()));
}

void SAKDebugPageOutputController::onShowMsCheckBoxClicked()
{
    mSettings->setValue(mSettingStringShowMs, QVariant::fromValue(mShowMsCheckBox->isChecked()));
}

void SAKDebugPageOutputController::onShowRxDataCheckBoxClicked()
{
    mSettings->setValue(mSettingStringShowRx, QVariant::fromValue(mShowRxDataCheckBox->isChecked()));
}

void SAKDebugPageOutputController::onShowTxDataCheckBoxClicked()
{
    mSettings->setValue(mSettingStringShowTx, QVariant::fromValue(mShowTxDataCheckBox->isChecked()));
}

void SAKDebugPageOutputController::onRawDataCheckBoxClicked()
{
    mSettings->setValue(mSettingStringRawData, QVariant::fromValue(mRawDataCheckBox->isChecked()));
}
