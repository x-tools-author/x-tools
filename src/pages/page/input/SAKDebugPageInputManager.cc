/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKInputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataItemManager.hh"
#include "SAKDebugPageInputManager.hh"
#include "SAKInputCrcSettingsDialog.hh"

SAKDebugPageInputManager::SAKDebugPageInputManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
{
    inputModelComboBox          = debugPage->mInputModelComboBox;
    cycleEnableCheckBox         = debugPage->mCycleEnableCheckBox;
    cycleTimeLineEdit           = debugPage->mCycleTimeLineEdit;
    saveInputDataPushButton     = debugPage->mSaveInputDataPushButton;
    readinFilePushButton        = debugPage->mReadinFilePushButton;
    addCRCCheckBox              = debugPage->mAddCRCCheckBox;
    clearInputPushButton        = debugPage->mClearInputPushButton;
    sendPushButton              = debugPage->mSendPushButton;
    inputTextEdit               = debugPage->mInputTextEdit;
    crcParameterModelsComboBox  = debugPage->mCrcParameterModelsComboBox;
    crcLabel                    = debugPage->mCrcLabel;
    presetPushButton            = debugPage->mPresetPushButton;
    sendPresetPushButton        = debugPage->mSendPresetPushButton;

    qRegisterMetaType<InputParameters>("InputParameters");
    inputDataFactory = new SAKInputDataFactory;
    inputDataFactory->start();

    crcInterface = new SAKCRCInterface;
    inputDataItemManager = new SAKInputDataItemManager(debugPage, this);
    crcSettingsDialog = new SAKInputCrcSettingsDialog;
    SAKInputCrcSettingsDialog::ParameterContext ctx = crcSettingsDialog->parametersContext();
    inputParameters.bigEndian = ctx.bigEndianCRC;
    inputParameters.startByte = ctx.startByte;
    inputParameters.endByte = ctx.endByte;

    /// @brief 弹窗参数更新后，更新输入参数
    connect(crcSettingsDialog, &SAKInputCrcSettingsDialog::parametersChanged, this, [&](){
        SAKInputCrcSettingsDialog::ParameterContext ctx = crcSettingsDialog->parametersContext();
        inputParameters.bigEndian = ctx.bigEndianCRC;
        inputParameters.startByte = ctx.startByte;
        inputParameters.endByte = ctx.endByte;
    });

    sendPushButton->setEnabled(false);
    sendPresetPushButton->setEnabled(false);
    cycleEnableCheckBox->setEnabled(false);
    SAKGlobal::initInputTextFormatComboBox(inputModelComboBox);
    SAKGlobal::initCRCComboBox(crcParameterModelsComboBox);

    connect(inputModelComboBox,         &QComboBox::currentTextChanged, this, &SAKDebugPageInputManager::changeInputModel);
    connect(cycleEnableCheckBox,        &QCheckBox::clicked,            this, &SAKDebugPageInputManager::changeCycleEnableFlag);
    connect(cycleTimeLineEdit,          &QLineEdit::textChanged,        this, &SAKDebugPageInputManager::changeCycleTime);
    connect(saveInputDataPushButton,    &QPushButton::clicked,          this, &SAKDebugPageInputManager::saveInputDataToFile);
    connect(readinFilePushButton,       &QPushButton::clicked,          this, &SAKDebugPageInputManager::readinFile);
    connect(addCRCCheckBox,             &QCheckBox::clicked,            this, &SAKDebugPageInputManager::changeAddCRCFlag);
    connect(clearInputPushButton,       &QPushButton::clicked,          this, &SAKDebugPageInputManager::clearInputArea);
    connect(sendPushButton,             &QPushButton::clicked,          this, &SAKDebugPageInputManager::sendRawData);
    connect(inputTextEdit,              &QTextEdit::textChanged,        this, &SAKDebugPageInputManager::inputTextEditTextChanged);
    connect(crcParameterModelsComboBox, &QComboBox::currentTextChanged, this, &SAKDebugPageInputManager::changeCRCModel);
    connect(presetPushButton,           &QPushButton::clicked,          this, &SAKDebugPageInputManager::setPresetData);
    connect(sendPresetPushButton,       &QPushButton::clicked,          this, &SAKDebugPageInputManager::sendPresetData);

    connect(this, &SAKDebugPageInputManager::rawDataChanged, inputDataFactory, &SAKInputDataFactory::cookData);
    connect(inputDataFactory, &SAKInputDataFactory::dataCooked, debugPage, &SAKDebugPage::write);
    connect(&timingTimer, &QTimer::timeout, this, &SAKDebugPageInputManager::cycleTimerTimeout);
    connect(debugPage, &SAKDebugPage::requestWriteRawData, this, &SAKDebugPageInputManager::sendOtherRawData);

    initParameters();
    updateCRC();
}

SAKDebugPageInputManager::~SAKDebugPageInputManager()
{
    delete inputDataFactory;
    delete crcInterface;
    delete inputDataItemManager;
    delete crcSettingsDialog;
}

void SAKDebugPageInputManager::showCrcSettingsDialog()
{
    crcSettingsDialog->show();
}

void SAKDebugPageInputManager::changeInputModel(const QString &text)
{
    /// @brief 在ui初始化的时候，会出现text为empty的情况
    if (text.isEmpty()){
        return;
    }

    bool ok = false;
    inputTextEdit->clear();
    inputParameters.inputModel = inputModelComboBox->currentData().toInt(&ok);
    Q_ASSERT_X(ok, __FUNCTION__, "Input model is error");
}

void SAKDebugPageInputManager::changeCycleEnableFlag()
{
    if (cycleEnableCheckBox->isChecked()){
        timingTimer.start(inputParameters.cycleTime);
    }else{
        timingTimer.stop();
    }
}

void SAKDebugPageInputManager::changeCycleTime()
{
    bool ok = false;
    inputParameters.cycleTime = cycleTimeLineEdit->text().toInt(&ok);
    if ((!ok) || (inputParameters.cycleTime == 0)){
        inputParameters.cycleTime = 1000;
        cycleTimeLineEdit->setText("1000");
    }
}

void SAKDebugPageInputManager::saveInputDataToFile()
{
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                    tr("保存文件"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+QString("input.txt"),
                                                    QString("Text (*.txt)"));

    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)){
        QByteArray data = inputTextEdit->toPlainText().toUtf8();
        qint64 ret = file.write(data);
        if (ret == -1){
            debugPage->outputMessage(file.errorString(), false);
        }
        file.close();
    }else {
        debugPage->outputMessage(file.errorString(), false);
    }
}

void SAKDebugPageInputManager::readinFile()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("打开文件"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly)){
            QByteArray data = file.readAll();
            inputTextEdit->setText(QString(data).toUtf8());
            file.close();
        }else{
            debugPage->outputMessage( QString("%1 %2").arg(tr("无法打开文件")).arg(fileName));
        }
    }
}

void SAKDebugPageInputManager::changeAddCRCFlag()
{
    inputParameters.addCRC = addCRCCheckBox->isChecked();
}

void SAKDebugPageInputManager::clearInputArea()
{
    inputTextEdit->clear();
}

void SAKDebugPageInputManager::inputTextEditTextChanged()
{
    formattingInputText(inputTextEdit, inputParameters.inputModel);

    updateCRC();
}

void SAKDebugPageInputManager::sendRawData()
{
    QString data = inputTextEdit->toPlainText();
    if (data.isEmpty()){
        data = QString("(empty)");
    }

    emit rawDataChanged(data, inputParameters);
}

void SAKDebugPageInputManager::sendOtherRawData(QString data, int textFormat)
{
    InputParameters temp = inputParameters;
    temp.inputModel = textFormat;

    emit rawDataChanged(data, temp);
}

void SAKDebugPageInputManager::changeCRCModel()
{
    bool ok = false;
    inputParameters.crcModel = crcParameterModelsComboBox->currentData().toInt(&ok);
    Q_ASSERT_X(ok, __FUNCTION__, "Please check the crc parameters model");

    updateCRC();
}

void SAKDebugPageInputManager::setPresetData()
{
    if (inputDataItemManager->isHidden()){
        inputDataItemManager->show();
    }else{
        inputDataItemManager->activateWindow();
    }
}

void SAKDebugPageInputManager::sendPresetData()
{

}

void SAKDebugPageInputManager::initParameters()
{
    inputParameters.addCRC = addCRCCheckBox->isChecked();
    inputParameters.cycleTime = cycleTimeLineEdit->text().toInt();

    bool ok = false;
    if ((!ok) || (inputParameters.cycleTime == 0)){
        inputParameters.cycleTime = 1000;
        cycleTimeLineEdit->setText("1000");
    }

    inputParameters.inputModel = inputModelComboBox->currentData().toInt();
    inputParameters.crcModel = crcParameterModelsComboBox->currentData().toInt();
}

void SAKDebugPageInputManager::setCycleEnable()
{
    if (cycleEnableCheckBox->isChecked()){
        /// @brief 如果输入框输入内容不符合规范，默认循环周期为50ms
        bool ok = false;
        int cycleTime = cycleTimeLineEdit->text().toInt(&ok);
        if (ok){
            cycleTimeLineEdit->setText("50");
            cycleTime = 50;
        }
        timingTimer.start(cycleTime);
        cycleTimeLineEdit->setEnabled(false);
    }else{
        timingTimer.stop();
        cycleTimeLineEdit->setEnabled(true);
    }
}

void SAKDebugPageInputManager::cycleTimerTimeout()
{
    sendRawData();
}

void SAKDebugPageInputManager::updateCRC()
{
    QString rawData = inputTextEdit->toPlainText();
    QByteArray cookedData = inputDataFactory->rawDataToArray(rawData, inputParameters);

    quint32 crc = inputDataFactory->crcCalculate(cookedData, inputParameters.crcModel);
    int bits = crcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(inputParameters.crcModel));
    crcLabel->setText(QString(QString("%1").arg(QString::number(crc, 16), (bits/8)*2, '0')).toUpper().prepend("0x"));
}

void SAKDebugPageInputManager::formattingInputText(QTextEdit *textEdit, int model)
{
    if (!textEdit){
        return;
    }

    textEdit->blockSignals(true);
    QString plaintext = textEdit->toPlainText();
    if (!plaintext.isEmpty()){
        if (model == SAKDataStruct::InputFormatBin){
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
        }else if(model == SAKDataStruct::InputFormatOct) {
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
        }else if(model == SAKDataStruct::InputFormatDec) {
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
        }else if(model == SAKDataStruct::InputFormatHex) {
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
        }else if(model == SAKDataStruct::InputFormatAscii) {
            QString newString;
            for (int i = 0; i < plaintext.count(); i++){
                if (plaintext.at(i).unicode() <= 127){
                    newString.append(plaintext.at(i));
                }
            }
            textEdit->setText(newString);
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKDataStruct::InputFormatUtf8) {
            /// nothing to do
        }else if(model == SAKDataStruct::InputFormatLocal) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow input model");
        }
    }
    textEdit->blockSignals(false);
}

