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
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKInputCrcSettingsDialog.hh"
#include "SAKDebugPageInputController.hh"
#include "SAKInputDataPresetItemManager.hh"

SAKDebugPageInputController::SAKDebugPageInputController(SAKDebugPage *debugPage, QObject *parent)
    :QObject (parent)
    ,mDebugPage (debugPage)
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

    qRegisterMetaType<InputParametersContext>("InputParameters");
    mInputDataFactory = new SAKInputDataFactory;
    mInputDataFactory->start();

    mCrcInterface = new SAKCRCInterface;

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
    });

    mSendPushButton->setEnabled(false);
    mSendPresetPushButton->setEnabled(false);
    mCycleEnableCheckBox->setEnabled(false);
    SAKGlobal::initInputTextFormatComboBox(mInputModelComboBox);
    SAKGlobal::initCRCComboBox(mCrcParameterModelsComboBox);

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
    delete  mCrcInterface;
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

void SAKDebugPageInputController::changeInputModel(const QString &text)
{
    // Ignore empty text
    if (text.isEmpty()){
        return;
    }

    bool ok = false;
    mInputTextEdit->clear();
    mInputParameters.inputModel = mInputModelComboBox->currentData().toInt(&ok);
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
    if ((!ok) || (mInputParameters.cycleTime == 0)){
        mInputParameters.cycleTime = 1000;
        mCycleTimeLineEdit->setText("1000");
    }
}

void SAKDebugPageInputController::saveInputDataToFile()
{
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                    tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation)+QString("input.txt"),
                                                    QString("Text (*.txt)"));

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

    bool ok = false;
    if ((!ok) || (mInputParameters.cycleTime == 0)){
        mInputParameters.cycleTime = 1000;
        mCycleTimeLineEdit->setText("1000");
    }

    mInputParameters.inputModel = mInputModelComboBox->currentData().toInt();
    mInputParameters.crcModel = mCrcParameterModelsComboBox->currentData().toInt();
}

void SAKDebugPageInputController::setCycleEnable()
{
    if (mCycleEnableCheckBox->isChecked()){
        /// @brief 如果输入框输入内容不符合规范，默认循环周期为50ms
        bool ok = false;
        int cycleTime = mCycleTimeLineEdit->text().toInt(&ok);
        if (ok){
            mCycleTimeLineEdit->setText("50");
            cycleTime = 50;
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
    int bits =  mCrcInterface->getBitsWidth(static_cast<SAKCRCInterface::CRCModel>(mInputParameters.crcModel));
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
