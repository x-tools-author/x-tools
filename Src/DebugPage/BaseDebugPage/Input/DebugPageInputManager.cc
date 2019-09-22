/*
 * Copyright (C) 2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include "SAKBase.hh"
#include "SAKDebugPage.hh"
#include "InputDataItem.hh"
#include "SAKCRCInterface.hh"
#include "InputDataFactory.hh"
#include "DebugPageInputManager.hh"

#include <QFile>
#include <QDebug>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>

DebugPageInputManager::DebugPageInputManager(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,debugPage (debugPage)
{   
    qRegisterMetaType<InputParameters>("InputParameters");
    inputDataFactory = new InputDataFactory;
    inputDataFactory->start();

    crcInterface = new SAKCRCInterface(this);

    inputModelComboBox          = debugPage->inputModelComboBox;
    cycleEnableCheckBox         = debugPage->cycleEnableCheckBox;
    cycleTimeLineEdit           = debugPage->cycleTimeLineEdit;
    saveInputDataPushButton     = debugPage->saveInputDataPushButton;
    readinFilePushButton        = debugPage->readinFilePushButton;
    addCRCCheckBox              = debugPage->addCRCCheckBox;
    bigeEndianCheckBox          = debugPage->bigeEndianCheckBox;
    clearInputPushButton        = debugPage->clearInputPushButton;
    sendPushButton              = debugPage->sendPushButton;
    inputTextEdit               = debugPage->inputTextEdit;
    crcParameterModelsComboBox  = debugPage->crcParameterModelsComboBox;
    crcLabel                    = debugPage->crcLabel;

    sendPushButton->setEnabled(false);
    SAKBase::instance()->initTextFormatComboBox(inputModelComboBox);
    SAKCRCInterface::initCRCComboBox(crcParameterModelsComboBox);

    connect(inputModelComboBox,         &QComboBox::currentTextChanged, this, &DebugPageInputManager::changeInputModel);
    connect(cycleEnableCheckBox,        &QCheckBox::clicked,            this, &DebugPageInputManager::changeCycleEnableFlag);
    connect(cycleTimeLineEdit,          &QLineEdit::textChanged,        this, &DebugPageInputManager::changeCycleTime);
    connect(saveInputDataPushButton,    &QPushButton::clicked,          this, &DebugPageInputManager::saveInputDataToFile);
    connect(readinFilePushButton,       &QPushButton::clicked,          this, &DebugPageInputManager::readinFile);
    connect(addCRCCheckBox,             &QCheckBox::clicked,            this, &DebugPageInputManager::changeAddCRCFlag);
    connect(bigeEndianCheckBox,         &QCheckBox::clicked,            this, &DebugPageInputManager::changeEndianFlag);
    connect(clearInputPushButton,       &QPushButton::clicked,          this, &DebugPageInputManager::clearInputArea);
    connect(sendPushButton,             &QPushButton::clicked,          this, &DebugPageInputManager::sendRawData);
    connect(inputTextEdit,              &QTextEdit::textChanged,        this, &DebugPageInputManager::inputTextEditTextChanged);
    connect(crcParameterModelsComboBox, &QComboBox::currentTextChanged, this, &DebugPageInputManager::changeCRCModel);

    connect(this, &DebugPageInputManager::rawDataChanged, inputDataFactory, &InputDataFactory::cookData);
    connect(inputDataFactory, &InputDataFactory::dataCooked, debugPage, &SAKDebugPage::write);
    connect(&timingTimer, &QTimer::timeout, this, &DebugPageInputManager::cycleTimerTimeout);

    initParameters();
    updateCRC();
}

DebugPageInputManager::~DebugPageInputManager()
{
    inputDataFactory->terminate();
    delete inputDataFactory;
}

void DebugPageInputManager::changeInputModel(const QString &text)
{
    /*
     *  在ui初始化的时候，会出现text为empty的情况
     */
    if (text.isEmpty()){
        return;
    }

    bool ok = false;
    inputTextEdit->clear();
    inputParameters.inputModel = inputModelComboBox->currentData().toInt(&ok);
    Q_ASSERT_X(ok, __FUNCTION__, "Input model is error");
}

void DebugPageInputManager::changeCycleEnableFlag()
{
    if (cycleEnableCheckBox->isChecked()){
        timingTimer.start(inputParameters.cycleTime);
    }else{
        timingTimer.stop();
    }
}

void DebugPageInputManager::changeCycleTime()
{
    bool ok = false;
    inputParameters.cycleTime = cycleTimeLineEdit->text().toInt(&ok);
    if ((!ok) || (inputParameters.cycleTime == 0)){
        debugPage->outputMessage(tr("循环发送周期设置有误，已设置1000ms"), false);
        inputParameters.cycleTime = 1000;
        cycleTimeLineEdit->setText("1000");
    }
}

void DebugPageInputManager::saveInputDataToFile()
{
    QString fileName = QFileDialog::getSaveFileName(nullptr,
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

void DebugPageInputManager::readinFile()
{
    QString fileName = QFileDialog::getOpenFileName(nullptr, tr("打开文件"));
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

void DebugPageInputManager::changeAddCRCFlag()
{
    inputParameters.addCRC = addCRCCheckBox->isChecked();
}

void DebugPageInputManager::changeEndianFlag()
{
    inputParameters.bigEndian = bigeEndianCheckBox->isChecked();
}

void DebugPageInputManager::clearInputArea()
{
    inputTextEdit->clear();
}

void DebugPageInputManager::inputTextEditTextChanged()
{
    formattingInputText(inputTextEdit, inputParameters.inputModel);

    updateCRC();
}

void DebugPageInputManager::sendRawData()
{
    QString data = inputTextEdit->toPlainText();
    if (data.isEmpty()){
        debugPage->outputMessage(tr("请输入发送数据后尝试"), false);
        return;
    }

    emit rawDataChanged(data, inputParameters);
}

void DebugPageInputManager::changeCRCModel()
{
    bool ok = false;
    inputParameters.crcModel = crcParameterModelsComboBox->currentData().toInt(&ok);
    Q_ASSERT_X(ok, __FUNCTION__, "Please check the crc parameters model");

    updateCRC();
}

void DebugPageInputManager::initParameters()
{
    inputParameters.addCRC = addCRCCheckBox->isChecked();
    inputParameters.bigEndian = bigeEndianCheckBox->isChecked();
    inputParameters.cycleTime = cycleTimeLineEdit->text().toInt();

    bool ok = false;
    if ((!ok) || (inputParameters.cycleTime == 0)){
        debugPage->outputMessage(tr("循环发送周期设置有误，已设置1000ms"));
        inputParameters.cycleTime = 1000;
        cycleTimeLineEdit->setText("1000");
    }

    inputParameters.inputModel = inputModelComboBox->currentData().toInt();
    inputParameters.crcModel = crcParameterModelsComboBox->currentData().toInt();
}

void DebugPageInputManager::setCycleEnable()
{
    if (cycleEnableCheckBox->isChecked()){
        /*
         * 如果输入框输入内容不符合规范，默认循环周期为50ms
         */
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

void DebugPageInputManager::cycleTimerTimeout()
{
    sendRawData();
}

void DebugPageInputManager::updateCRC()
{
    QString rawData = inputTextEdit->toPlainText();
    QByteArray cookedData = inputDataFactory->rawDataToArray(rawData, inputParameters);
    quint32 crc = inputDataFactory->crcCalculate(cookedData, inputParameters.crcModel);

    int bits = crcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(inputParameters.crcModel));
    crcLabel->setText(QString(QString("%1").arg(QString::number(crc, 16), (bits/8)*2, '0')).toUpper().prepend("0x"));
}

void DebugPageInputManager::formattingInputText(QTextEdit *textEdit, int model)
{
    if (!textEdit){
        return;
    }

    textEdit->blockSignals(true);
    QString plaintext = textEdit->toPlainText();
    if (!plaintext.isEmpty()){
        if (model == SAKBase::Bin){
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
        }else if(model == SAKBase::Oct) {
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
        }else if(model == SAKBase::Dec) {
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
        }else if(model == SAKBase::Hex) {
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
        }else if(model == SAKBase::Ascii) {
            plaintext.remove(QRegExp("[^\0u00-\u007f ]"));
            textEdit->setText(plaintext);
            textEdit->moveCursor(QTextCursor::End);
        }else if(model == SAKBase::Local) {
            /// nothing to do
        }else {
            Q_ASSERT_X(false, __FUNCTION__, "Unknow input model");
        }
    }
    textEdit->blockSignals(false);
}

