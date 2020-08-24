/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
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
#include <QAction>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKInputCrcSettingsDialog.hh"
#include "SAKDebugPageInputController.hh"
#include "SAKInputDataPresetItemManager.hh"

SAKDebugPageInputController::SAKDebugPageInputController(SAKDebugPage *debugPage, QObject *parent)
    :QObject(parent)
    ,mDebugPage(debugPage)
{
    mInputModelComboBox = debugPage->mInputModelComboBox;
    mCycleEnableCheckBox = debugPage->mCycleEnableCheckBox;
    mCycleTimeLineEdit = debugPage->mCycleTimeLineEdit;
    mSaveInputDataPushButton = debugPage->mSaveInputDataPushButton;
    mReadinFilePushButton = debugPage->mReadinFilePushButton;
    mAddCRCCheckBox = debugPage->mAddCRCCheckBox;
    mClearInputPushButton = debugPage->mClearInputPushButton;
    mSendPushButton = debugPage->mSendPushButton;
    mInputTextEdit = debugPage->mInputTextEdit;
    mCrcParameterModelsComboBox = debugPage->mCrcParameterModelsComboBox;
    mCrcLabel = debugPage->mCrcLabel;
    mPresetPushButton = debugPage->mPresetPushButton;
    mSendPresetPushButton = debugPage->mSendPresetPushButton;
    mSendPresetPushButton->setMenu(new QMenu("", mSendPresetPushButton));
    mCrcSettingsPushButton = debugPage->mCrcSettingsPushButton;

    // Initializing variables about settings
    QString group = mDebugPage->settingsGroup();
    mSettings = mDebugPage->settings();
    Q_ASSERT_X(mSettings, __FUNCTION__, "Value can not be null!");
    mSettingStringInputTextFromat = QString("%1/inputTextFormat").arg(group);
    mSettingStringCycleTime = QString("%1/cycleTime").arg(group);
    mSettingStringAddCRC = QString("%1/addCRC").arg(group);
    mSettingStringBigEndian = QString("%1/bigEndian").arg(group);
    mSettingStringCrcParameterModel = QString("%1/parameterModel").arg(group);
    mSettingStringCrcStartByte = QString("%1/startByte").arg(group);
    mSettingStringCrcEndByte = QString("%1/endByte").arg(group);

    // InputParametersContext will be a parameter of signal,
    // so, do something make compiling happy
    qRegisterMetaType<InputParametersContext>("InputParameters");
    mInputDataFactory = new SAKInputDataFactory;
    mInputDataFactory->start();

    mCrcInterface = new SAKCommonCrcInterface;

    // Add actions after new.
    mInputDataItemManager = new SAKInputDataPresetItemManager(debugPage);
    QList<SAKInputDataPresetItem*> list = mInputDataItemManager->itemList();
    for (auto var : list){
        appendAction(var);
    }

    mCrcSettingsDialog = new SAKInputCrcSettingsDialog;

    SAKInputCrcSettingsDialog::ParameterContext ctx = mCrcSettingsDialog->parametersContext();
    mInputParameters.bigEndian = ctx.bigEndianCRC;
    mInputParameters.startByte = ctx.startByte;
    mInputParameters.endByte = ctx.endByte;

    // Disable some components before device is opened
    mSendPushButton->setEnabled(false);
    mSendPresetPushButton->setEnabled(false);
    mCycleEnableCheckBox->setEnabled(false);
    SAKGlobal::initInputTextFormatComboBox(mInputModelComboBox);
    SAKGlobal::initCRCComboBox(mCrcParameterModelsComboBox);
    // The function must be called before connecting signals and slots
    readinSettings();

    // Preset items changed
    connect(mInputDataItemManager, &SAKInputDataPresetItemManager::itemAdded, this, &SAKDebugPageInputController::appendAction);
    connect(mInputDataItemManager, &SAKInputDataPresetItemManager::itemDeleted, this, &SAKDebugPageInputController::removeAction);
    connect(mInputDataItemManager, &SAKInputDataPresetItemManager::descriptionChanged, this, &SAKDebugPageInputController::changeDescription);

    // Update parameters
    connect(mCrcSettingsDialog, &SAKInputCrcSettingsDialog::parametersChanged, this, [&](){
        SAKInputCrcSettingsDialog::ParameterContext ctx = mCrcSettingsDialog->parametersContext();
        mInputParameters.bigEndian = ctx.bigEndianCRC;
        mInputParameters.startByte = ctx.startByte;
        mInputParameters.endByte = ctx.endByte;

        // Write value to setting file
        mSettings->setValue(mSettingStringBigEndian, QVariant::fromValue(mInputParameters.bigEndian));
        mSettings->setValue(mSettingStringCrcStartByte, QVariant::fromValue(mInputParameters.startByte));
        mSettings->setValue(mSettingStringCrcEndByte, QVariant::fromValue(mInputParameters.endByte));
    });

    connect(mInputModelComboBox, &QComboBox::currentTextChanged, this, &SAKDebugPageInputController::changeInputModel);
    connect(mCycleEnableCheckBox, &QCheckBox::clicked, this, &SAKDebugPageInputController::changeCycleEnableFlag);
    connect(mCycleTimeLineEdit, &QLineEdit::textChanged, this, &SAKDebugPageInputController::changeCycleTime);
    connect(mSaveInputDataPushButton, &QPushButton::clicked, this, &SAKDebugPageInputController::saveInputDataToFile);
    connect(mReadinFilePushButton, &QPushButton::clicked, this, &SAKDebugPageInputController::readinFile);
    connect(mAddCRCCheckBox, &QCheckBox::clicked, this, &SAKDebugPageInputController::changeAddCRCFlag);
    connect(mClearInputPushButton, &QPushButton::clicked, this, &SAKDebugPageInputController::clearInputArea);
    connect(mSendPushButton, &QPushButton::clicked, this, &SAKDebugPageInputController::sendRawData);
    connect(mInputTextEdit, &QTextEdit::textChanged, this, &SAKDebugPageInputController::inputTextEditTextChanged);
    connect(mCrcParameterModelsComboBox, &QComboBox::currentTextChanged, this, &SAKDebugPageInputController::changeCRCModel);
    connect(mPresetPushButton, &QPushButton::clicked, this, &SAKDebugPageInputController::setPresetData);
    connect(mCrcSettingsPushButton, &QPushButton::clicked, this, &SAKDebugPageInputController::showCrcSettingsDialog);
    connect(this, &SAKDebugPageInputController::rawDataChanged, mInputDataFactory, &SAKInputDataFactory::cookData);
    connect(mInputDataFactory, &SAKInputDataFactory::dataCooked, debugPage, &SAKDebugPage::write);
    connect(&mTimingTimer, &QTimer::timeout, this, &SAKDebugPageInputController::cycleTimerTimeout);
    connect(debugPage, &SAKDebugPage::requestWriteRawData, this, &SAKDebugPageInputController::sendOtherRawData);

    initParameters();
    updateCRC();
}

SAKDebugPageInputController::~SAKDebugPageInputController()
{
    delete mInputDataFactory;
    delete mCrcInterface;
    delete mInputDataItemManager;
    delete mCrcSettingsDialog;
}

void SAKDebugPageInputController::showCrcSettingsDialog()
{
    mCrcSettingsDialog->show();
}

void SAKDebugPageInputController::formattingInputText(QTextEdit *textEdit, int model)
{
    if (!textEdit){
        return;
    }

    textEdit->blockSignals(true);
    QString plaintext = textEdit->toPlainText();
    if (!plaintext.isEmpty()){
        if (model == SAKCommonDataStructure::InputFormatBin){
            QString strTemp;
            plaintext.remove(QRegExp("[^0-1]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 8 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            textEdit->setText(strTemp);
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKCommonDataStructure::InputFormatOct) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-7]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            textEdit->setText(strTemp);
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKCommonDataStructure::InputFormatDec) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            textEdit->setText(strTemp);
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKCommonDataStructure::InputFormatHex) {
            QString strTemp;
            plaintext.remove(QRegExp("[^0-9a-fA-F]"));
            for (int i = 0; i < plaintext.length(); i++){
                if ((i != 0) && (i % 2 == 0)){
                    strTemp.append(QChar(' '));
                }
                strTemp.append(plaintext.at(i));
            }
            textEdit->setText(strTemp.toUpper());
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKCommonDataStructure::InputFormatAscii) {
            QString newString;
            for (int i = 0; i < plaintext.count(); i++){
                if (plaintext.at(i).unicode() <= 127){
                    newString.append(plaintext.at(i));
                }
            }
            textEdit->setText(newString);
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKCommonDataStructure::InputFormatUtf8) {
            /// nothing to do
        }else if(model == SAKCommonDataStructure::InputFormatLocal) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow input model");
        }
    }
    textEdit->blockSignals(false);
}

void SAKDebugPageInputController::changeInputModel(const QString &text)
{
    // Ignore empty text
    if (text.isEmpty()){
        return;
    }

    bool ok = false;
    mInputTextEdit->clear();
    mInputParameters.inputModel = mInputModelComboBox->currentData().toInt(&ok);
    mSettings->setValue(mSettingStringInputTextFromat, QVariant::fromValue(mInputModelComboBox->currentData().toInt()));
    Q_ASSERT_X(ok, __FUNCTION__, "Input model is error");
}

void SAKDebugPageInputController::changeCycleEnableFlag()
{
    if (mCycleEnableCheckBox->isChecked()){
        mTimingTimer.start(mInputParameters.cycleTime);
    }else{
        mTimingTimer.stop();
    }
}

void SAKDebugPageInputController::changeCycleTime()
{
    bool ok = false;
    mInputParameters.cycleTime = mCycleTimeLineEdit->text().toInt(&ok);
    mSettings->setValue(mSettingStringCycleTime, QVariant::fromValue(mInputParameters.cycleTime));
    if ((!ok) || (mInputParameters.cycleTime < 0)){
        mInputParameters.cycleTime = 1000;
        mCycleTimeLineEdit->setText("1000");
    }
}

void SAKDebugPageInputController::saveInputDataToFile()
{
    QString defaultName = QDateTime::currentDateTime().toString("yyyyMMdd").append(".txt");
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                    tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append(defaultName),
                                                    tr("txt (*.txt);;all (*.txt)"));

    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)){
        QByteArray data = mInputTextEdit->toPlainText().toUtf8();
        qint64 ret = file.write(data);
        if (ret == -1){
            mDebugPage->outputMessage(file.errorString(), false);
        }
        file.close();
    }else {
        mDebugPage->outputMessage(file.errorString(), false);
    }
}

void SAKDebugPageInputController::readinFile()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open File"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly)){
            QByteArray data = file.readAll();
            mInputTextEdit->setText(QString(data).toUtf8());
            file.close();
        }else{
            mDebugPage->outputMessage(QString("%1:%2").arg(tr("Can not open file")).arg(fileName));
        }
    }
}

void SAKDebugPageInputController::changeAddCRCFlag()
{
    mInputParameters.addCRC = mAddCRCCheckBox->isChecked();
    mSettings->setValue(mSettingStringAddCRC, QVariant::fromValue(mAddCRCCheckBox->isChecked()));
}

void SAKDebugPageInputController::clearInputArea()
{
    mInputTextEdit->clear();
}

void SAKDebugPageInputController::inputTextEditTextChanged()
{
    formattingInputText(mInputTextEdit, mInputParameters.inputModel);

    updateCRC();
}

void SAKDebugPageInputController::sendRawData()
{
    QString data = mInputTextEdit->toPlainText();
    if (data.isEmpty()){
        data = QString("(empty)");
    }

    emit rawDataChanged(data, mInputParameters);
}

void SAKDebugPageInputController::sendOtherRawData(QString data, int textFormat)
{
    InputParametersContext temp = mInputParameters;
    temp.inputModel = textFormat;

    emit rawDataChanged(data, temp);
}

void SAKDebugPageInputController::changeCRCModel()
{
    bool ok = false;
    mInputParameters.crcModel = mCrcParameterModelsComboBox->currentData().toInt(&ok);
    mSettings->setValue(mSettingStringCrcParameterModel, QVariant::fromValue(mCrcParameterModelsComboBox->currentIndex()));
    Q_ASSERT_X(ok, __FUNCTION__, "Please check the crc parameters model");

    updateCRC();
}

void SAKDebugPageInputController::setPresetData()
{
    if (mInputDataItemManager->isHidden()){
        mInputDataItemManager->show();
    }else{
        mInputDataItemManager->activateWindow();
    }
}

void SAKDebugPageInputController::initParameters()
{
    mInputParameters.addCRC = mAddCRCCheckBox->isChecked();
    mInputParameters.cycleTime = mCycleTimeLineEdit->text().toInt();
    mInputParameters.inputModel = mInputModelComboBox->currentData().toInt();
    mInputParameters.crcModel = mCrcParameterModelsComboBox->currentData().toInt();
}

void SAKDebugPageInputController::setCycleEnable()
{
    if (mCycleEnableCheckBox->isChecked()){
        // If the cycle is error, default value will be used.
        bool ok = false;
        int cycleTime = mCycleTimeLineEdit->text().toInt(&ok);
        if (!ok){
            mCycleTimeLineEdit->setText(QString::number(defaultCycle));
            cycleTime = defaultCycle;
        }

        mTimingTimer.start(cycleTime);
        mCycleTimeLineEdit->setEnabled(false);
    }else{
        mTimingTimer.stop();
        mCycleTimeLineEdit->setEnabled(true);
    }
}

void SAKDebugPageInputController::cycleTimerTimeout()
{
    sendRawData();
}

void SAKDebugPageInputController::updateCRC()
{
    QString rawData = mInputTextEdit->toPlainText();
    QByteArray cookedData = mInputDataFactory->rawDataToArray(rawData, mInputParameters);

    quint32 crc = mInputDataFactory->crcCalculate(cookedData, mInputParameters.crcModel);
    int bits =  mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(mInputParameters.crcModel));
    mCrcLabel->setText(QString(QString("%1").arg(QString::number(crc, 16), (bits/8)*2, '0')).toUpper().prepend("0x"));
}

void SAKDebugPageInputController::appendAction(SAKInputDataPresetItem *item)
{
    QString description = item->itemDescription();
    QAction *action = new QAction(description, mSendPresetPushButton);
    action->setData(QVariant::fromValue(item));
    mSendPresetPushButton->menu()->addAction(action);
    connect(action, &QAction::triggered, this, &SAKDebugPageInputController::actionTriggered);
}

void SAKDebugPageInputController::removeAction(SAKInputDataPresetItem *item)
{
    QList<QAction*> actionList = mSendPresetPushButton->menu()->actions();
    for (auto var : actionList){
        if (var->data().value<SAKInputDataPresetItem*>() == item){
            var->deleteLater();
            break;
        }
    }
}

void SAKDebugPageInputController::changeDescription(SAKInputDataPresetItem *item)
{
    QList<QAction*> actionList = mSendPresetPushButton->menu()->actions();
    for (auto var : actionList){
        if (var->data().value<SAKInputDataPresetItem*>() == item){
            var->setText(item->itemDescription());
            break;
        }
    }
}

void SAKDebugPageInputController::actionTriggered()
{
    if (sender()){
        if (sender()->inherits("QAction")){
            QAction *action = qobject_cast<QAction*>(sender());
            SAKInputDataPresetItem *item = action->data().value<SAKInputDataPresetItem *>();
            int format = item->itemTextFromat();
            QString text = item->itemText();

            sendOtherRawData(text, format);
        }
    }
}

void SAKDebugPageInputController::readinSettings()
{
    QVariant var = mSettings->value(mSettingStringInputTextFromat);
    int index = 0;
    if (var.isNull()){
        index = 4;
    }else{
        index = var.toInt();
    }
    mInputModelComboBox->setCurrentIndex(index);

    var = mSettings->value(mSettingStringCycleTime);
    QString cycleTime;
    if (var.isNull()){
        cycleTime = QString::number(defaultCycle);
    }else{
        cycleTime = QString::number(var.toInt());
    }
    mCycleTimeLineEdit->setText(cycleTime);

    bool value = mSettings->value(mSettingStringAddCRC).toBool();
    mAddCRCCheckBox->setChecked(value);

    index = mSettings->value(mSettingStringCrcParameterModel).toInt();
    mCrcParameterModelsComboBox->setCurrentIndex(index);

    bool bigEndian = mSettings->value(mSettingStringBigEndian).toBool();
    mCrcSettingsDialog->setBigEndian(bigEndian);

    int startByte = mSettings->value(mSettingStringCrcStartByte).toInt();
    mCrcSettingsDialog->setStartByte(startByte);

    int endByte = mSettings->value(mSettingStringCrcEndByte).toInt();
    mCrcSettingsDialog->setEndByte(endByte);
}
