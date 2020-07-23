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
#include "SAKDebugPageInputController.hh"
#include "SAKInputCrcSettingsDialog.hh"

SAKDebugPageInputController::SAKDebugPageInputController(SAKDebugPage *debugPage, QObject *parent)
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

    connect(inputModelComboBox,         &QComboBox::currentTextChanged, this, &SAKDebugPageInputController::changeInputModel);
    connect(cycleEnableCheckBox,        &QCheckBox::clicked,            this, &SAKDebugPageInputController::changeCycleEnableFlag);
    connect(cycleTimeLineEdit,          &QLineEdit::textChanged,        this, &SAKDebugPageInputController::changeCycleTime);
    connect(saveInputDataPushButton,    &QPushButton::clicked,          this, &SAKDebugPageInputController::saveInputDataToFile);
    connect(readinFilePushButton,       &QPushButton::clicked,          this, &SAKDebugPageInputController::readinFile);
    connect(addCRCCheckBox,             &QCheckBox::clicked,            this, &SAKDebugPageInputController::changeAddCRCFlag);
    connect(clearInputPushButton,       &QPushButton::clicked,          this, &SAKDebugPageInputController::clearInputArea);
    connect(sendPushButton,             &QPushButton::clicked,          this, &SAKDebugPageInputController::sendRawData);
    connect(inputTextEdit,              &QTextEdit::textChanged,        this, &SAKDebugPageInputController::inputTextEditTextChanged);
    connect(crcParameterModelsComboBox, &QComboBox::currentTextChanged, this, &SAKDebugPageInputController::changeCRCModel);
    connect(presetPushButton,           &QPushButton::clicked,          this, &SAKDebugPageInputController::setPresetData);
    connect(sendPresetPushButton,       &QPushButton::clicked,          this, &SAKDebugPageInputController::sendPresetData);

    connect(this, &SAKDebugPageInputController::rawDataChanged, inputDataFactory, &SAKInputDataFactory::cookData);
    connect(inputDataFactory, &SAKInputDataFactory::dataCooked, debugPage, &SAKDebugPage::write);
    connect(&timingTimer, &QTimer::timeout, this, &SAKDebugPageInputController::cycleTimerTimeout);
    connect(debugPage, &SAKDebugPage::requestWriteRawData, this, &SAKDebugPageInputController::sendOtherRawData);

    initParameters();
    updateCRC();
}

SAKDebugPageInputController::~SAKDebugPageInputController()
{
    delete inputDataFactory;
    delete crcInterface;
    delete inputDataItemManager;
    delete crcSettingsDialog;
}

void SAKDebugPageInputController::showCrcSettingsDialog()
{
    crcSettingsDialog->show();
}

void SAKDebugPageInputController::changeInputModel(const QString &text)
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

void SAKDebugPageInputController::changeCycleEnableFlag()
{
    if (cycleEnableCheckBox->isChecked()){
        timingTimer.start(inputParameters.cycleTime);
    }else{
        timingTimer.stop();
    }
}

void SAKDebugPageInputController::changeCycleTime()
{
    bool ok = false;
    inputParameters.cycleTime = cycleTimeLineEdit->text().toInt(&ok);
    if ((!ok) || (inputParameters.cycleTime == 0)){
        inputParameters.cycleTime = 1000;
        cycleTimeLineEdit->setText("1000");
    }
}

void SAKDebugPageInputController::saveInputDataToFile()
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

void SAKDebugPageInputController::readinFile()
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

void SAKDebugPageInputController::changeAddCRCFlag()
{
    inputParameters.addCRC = addCRCCheckBox->isChecked();
}

void SAKDebugPageInputController::clearInputArea()
{
    inputTextEdit->clear();
}

void SAKDebugPageInputController::inputTextEditTextChanged()
{
    formattingInputText(inputTextEdit, inputParameters.inputModel);

    updateCRC();
}

void SAKDebugPageInputController::sendRawData()
{
    QString data = inputTextEdit->toPlainText();
    if (data.isEmpty()){
        data = QString("(empty)");
    }

    emit rawDataChanged(data, inputParameters);
}

void SAKDebugPageInputController::sendOtherRawData(QString data, int textFormat)
{
    InputParameters temp = inputParameters;
    temp.inputModel = textFormat;

    emit rawDataChanged(data, temp);
}

void SAKDebugPageInputController::changeCRCModel()
{
    bool ok = false;
    inputParameters.crcModel = crcParameterModelsComboBox->currentData().toInt(&ok);
    Q_ASSERT_X(ok, __FUNCTION__, "Please check the crc parameters model");

    updateCRC();
}

void SAKDebugPageInputController::setPresetData()
{
    if (inputDataItemManager->isHidden()){
        inputDataItemManager->show();
    }else{
        inputDataItemManager->activateWindow();
    }
}

void SAKDebugPageInputController::sendPresetData()
{

}

void SAKDebugPageInputController::initParameters()
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

void SAKDebugPageInputController::setCycleEnable()
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

void SAKDebugPageInputController::cycleTimerTimeout()
{
    sendRawData();
}

void SAKDebugPageInputController::updateCRC()
{
    QString rawData = inputTextEdit->toPlainText();
    QByteArray cookedData = inputDataFactory->rawDataToArray(rawData, inputParameters);

    quint32 crc = inputDataFactory->crcCalculate(cookedData, inputParameters.crcModel);
    int bits = crcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(inputParameters.crcModel));
    crcLabel->setText(QString(QString("%1").arg(QString::number(crc, 16), (bits/8)*2, '0')).toUpper().prepend("0x"));
}

void SAKDebugPageInputController::formattingInputText(QTextEdit *textEdit, int model)
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

