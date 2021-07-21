/*
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
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
#include <QtEndian>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKDebugger.hh"
#include "SAKDebuggerInput.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKInputCrcSettingsDialog.hh"
#include "SAKInputDataPresetItemManager.hh"

SAKDebuggerInput::SAKDebuggerInput(QComboBox *regularlySending,
                                   QComboBox *inputFormat,
                                   QPushButton *more,
                                   QPushButton *send,
                                   QLabel *crc,
                                   QTextEdit *input,
                                   QSettings *settings,
                                   const QString &settingsGroup,
                                   QObject *parent)
    :QThread(parent)
    ,mCyclingTimeComboBox(regularlySending)
    ,mInputModelComboBox(inputFormat)
    ,mMoreInputSettingsPushButton(more)
    ,mSendPushButton(send)
    ,mInputTextEdit(input)
    ,mCrcLabel(crc)
    ,mCrcInterface(new SAKCommonCrcInterface)
{
    mCyclingTimeComboBox->addItem(tr("Forbidden"), INT_MAX);
    QString unit = tr("ms");
    for (int i = 50; i < 500; i += 50){
        mCyclingTimeComboBox->addItem(QString::number(i) + QString(" ") + unit, i);
    }
    for (int i = 1000; i <= 5000; i += 1000){
        mCyclingTimeComboBox->addItem(QString::number(i) + QString(" ") + unit, i);
    }

    connect(mCyclingTimeComboBox, &QComboBox::currentTextChanged, this, [=](const QString &text){
        Q_UNUSED(text);
        mCyclingWritingTimer.setInterval(mCyclingTimeComboBox->currentData().toInt());
        mCyclingWritingTimer.start();
    });

    // Initialize more input settings button menu
    mMoreInputSettingsPushButton->setToolTip(tr("More input settings"));
    auto moreInputSettingsPushButtonMenu = new QMenu;
    connect(this, &QObject::destroyed, moreInputSettingsPushButtonMenu, &QMenu::deleteLater);
    mMoreInputSettingsPushButton->setMenu(moreInputSettingsPushButtonMenu);

    mWriteDataItemMenu = new QMenu(tr("Writing Data Items"));
    moreInputSettingsPushButtonMenu->addMenu(mWriteDataItemMenu);
    auto ret = moreInputSettingsPushButtonMenu->addSeparator();
    Q_UNUSED(ret);

    auto writeDataItemSettingsAction = new QAction(tr("Writing Data Item Settings"), this);
    connect(writeDataItemSettingsAction, &QAction::triggered, this, [=](){mInputDataItemManager->show();});
    moreInputSettingsPushButtonMenu->addAction(writeDataItemSettingsAction);

    auto clearInputAction = new QAction(tr("Clear Input"), this);
    connect(clearInputAction, &QAction::triggered, this, [=](){mInputTextEdit->clear();});
    moreInputSettingsPushButtonMenu->addAction(clearInputAction);

    auto readinFileAction = new QAction(tr("Readin File"), this);
    connect(readinFileAction, &QAction::triggered, this, [=](){readinFile();});
    moreInputSettingsPushButtonMenu->addAction(readinFileAction);

    auto saveInputDataAction = new QAction(tr("Save Input Data"), this);
    connect(saveInputDataAction, &QAction::triggered, this, [=](){saveInputDataToFile();});
    moreInputSettingsPushButtonMenu->addAction(saveInputDataAction);

    auto crcSettingsAction = new QAction(tr("CRC Settings"), this);
    connect(crcSettingsAction, &QAction::triggered, this, [=](){mCrcSettingsDialog->show();});
    moreInputSettingsPushButtonMenu->addAction(crcSettingsAction);

    // Initializing variables about settings
    QString group = settingsGroup;
    mSettings = settings;
    Q_ASSERT_X(mSettings, __FUNCTION__, "Value can not be null!");
    mSettingStringInputTextFromat = QString("%1/inputTextFormat").arg(group);

    // InputParametersContext will be a parameter of signal,
    // so, do something make compiling happy
    qRegisterMetaType<SAKStructInputParametersContext>("InputParameters");

    // Add actions after new.
#if 0
    mInputDataItemManager = new SAKInputDataPresetItemManager(debugPage);
    QList<SAKInputDataPresetItem*> list = mInputDataItemManager->itemList();
    for (auto &var : list){
        appendAction(var);
    }
#endif

    mCrcSettingsDialog = new SAKInputCrcSettingsDialog(settingsGroup, settings);
    SAKInputCrcSettingsDialog::SAKStructCrcParametersContext ctx = mCrcSettingsDialog->parametersContext();
    mInputParameters.crc.parametersModel = ctx.parameterMoldel;
    mInputParameters.crc.appending = ctx.append;
    mInputParameters.crc.bigEndian = ctx.bigEndian;
    mInputParameters.crc.startByte = ctx.startByte;
    mInputParameters.crc.endByte = ctx.endByte;

    // Disable some components before device is opened
    mSendPushButton->setEnabled(false);
    mCyclingTimeComboBox->setEnabled(false);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mInputModelComboBox);

    // The function must be called before connecting signals and slots
    readinSettings();
#if 0
    // Preset items changed
    connect(mInputDataItemManager, &SAKInputDataPresetItemManager::itemAdded, this, &SAKDebuggerInput::appendAction);
    connect(mInputDataItemManager, &SAKInputDataPresetItemManager::itemDeleted, this, &SAKDebuggerInput::removeAction);
    connect(mInputDataItemManager, &SAKInputDataPresetItemManager::descriptionChanged, this, &SAKDebuggerInput::changeDescription);
#endif

    // Update parameters
    connect(mCrcSettingsDialog, &SAKInputCrcSettingsDialog::crcParametersChanged, this, [&](){
        SAKInputCrcSettingsDialog::SAKStructCrcParametersContext ctx = mCrcSettingsDialog->parametersContext();
        mInputParameters.crc.parametersModel = ctx.parameterMoldel;
        mInputParameters.crc.appending = ctx.append;
        mInputParameters.crc.bigEndian = ctx.bigEndian;
        mInputParameters.crc.startByte = ctx.startByte;
        mInputParameters.crc.endByte = ctx.endByte;

        updateCrc();
    });

    connect(mInputModelComboBox, &QComboBox::currentTextChanged, this, &SAKDebuggerInput::changeInputModel);
    connect(mSendPushButton, &QPushButton::clicked, this, &SAKDebuggerInput::sendRawData);
    connect(mInputTextEdit, &QTextEdit::textChanged, this, &SAKDebuggerInput::inputTextEditTextChanged);
    //connect(mInputDataFactory, &SAKInputDataFactory::dataCooked, debugPage, &SAKDebugger::write);
    connect(&mCyclingWritingTimer, &QTimer::timeout, this, &SAKDebuggerInput::cyclingWritingTimerTimeout);
    //connect(debugPage, &SAKDebugger::requestWriteRawData, this, &SAKDebuggerInput::sendOtherRawData);

    initParameters();
    updateCrc();

    connect(this, &QThread::finished, this, [&](){
        mBytesInfoVectorMutex.lock();
        if (!mBytesInfoVector.isEmpty()) {
            start();
        }
        mBytesInfoVectorMutex.unlock();
    });
}

SAKDebuggerInput::~SAKDebuggerInput()
{
    delete mCrcInterface;
    //delete mInputDataItemManager;
    delete mCrcSettingsDialog;
}

void SAKDebuggerInput::showCrcSettingsDialog()
{
    mCrcSettingsDialog->show();
}

void SAKDebuggerInput::formattingInputText(QTextEdit *textEdit, int model)
{
    if (!textEdit){
        return;
    }

    textEdit->blockSignals(true);
    QString plaintext = textEdit->toPlainText();
    if (!plaintext.isEmpty()){
        QString cookedString = SAKCommonDataStructure::formattingString(plaintext, static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(model));
        textEdit->setText(cookedString);
        textEdit->moveCursor(QTextCursor::End);
    }
    textEdit->blockSignals(false);
}

void SAKDebuggerInput::inputBytes(QString rawBytes, SAKStructInputParametersContext parametersContext)
{
    if (!rawBytes.isEmpty()) {
        mBytesInfoVectorMutex.lock();
        QPair<QString, SAKStructInputParametersContext> pair(rawBytes, parametersContext);
        mBytesInfoVector.append(pair);
        if (!isRunning()) {
            start();
        }
        mBytesInfoVectorMutex.unlock();
    }
}

void SAKDebuggerInput::run()
{
    auto crcCalculate = [=](QByteArray data, int model)->quint32 {
        int bitsWidth = mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(model));
        switch (bitsWidth) {
        case 8:
            return mCrcInterface->crcCalculate<uint8_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCommonCrcInterface::CRCModel>(model));
        case 16:
            return mCrcInterface->crcCalculate<uint16_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCommonCrcInterface::CRCModel>(model));
        case 32:
            return mCrcInterface->crcCalculate<uint32_t>(reinterpret_cast<uint8_t*>(data.data()), static_cast<quint64>(data.length()), static_cast<SAKCommonCrcInterface::CRCModel>(model));
        default:
            return 0;
        }
    };


    auto extractCrcData = [](QByteArray crcData, SAKStructInputParametersContext parameters)->QByteArray {
        QByteArray crcInputData;
        int startIndex = parameters.crc.startByte - 1;
        startIndex = startIndex < 0 ? 0 : startIndex;

        int endIndex = (crcData.length() - 1) - (parameters.crc.endByte - 1);
        endIndex = endIndex < 0 ? 0 : endIndex;

        if (((crcData.length() - 1) >= startIndex) && ((crcData.length() - 1) >= endIndex)){
            int length = endIndex - startIndex + 1;
            length = length < 0 ? 0 : length;
            crcInputData = QByteArray(crcData.constData()+startIndex, length);
        }else{
            crcInputData = crcData;
        }
        return crcInputData;
    };

    auto takeBytesInfo = [&]()->auto {
        QPair<QString, SAKDebuggerInput::SAKStructInputParametersContext> pair;
        mBytesInfoVectorMutex.lock();
        if (!mBytesInfoVector.isEmpty()) {
            pair = mBytesInfoVector.takeFirst();
        }
        mBytesInfoVectorMutex.unlock();
        return pair;
    };

    QPair<QString, SAKStructInputParametersContext> pair = takeBytesInfo();
    QString rawData = pair.first;
    SAKStructInputParametersContext ctx = pair.second;
    auto textFormat = static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(ctx.textFormat);
    QByteArray cookedData = SAKCommonDataStructure::stringToByteArray(rawData, textFormat);


    if (ctx.crc.appending){
        QByteArray crcInputData = extractCrcData(cookedData, ctx);
        // Calculate the crc value of input data
        uint32_t crc  = crcCalculate(crcInputData, ctx.crc.parametersModel);
        uint8_t crc8  = static_cast<uint8_t>(crc);
        uint16_t crc16 = static_cast<uint16_t>(crc);
        int bitsWidth = mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(ctx.crc.parametersModel));
        if (ctx.crc.bigEndian){
            crc16 = qToBigEndian(crc16);
            crc = qToBigEndian(crc);
        }

        // Append crc byte to data
        switch (bitsWidth) {
            case 8: cookedData.append(reinterpret_cast<char*>(&crc8), 1); break;
            case 16: cookedData.append(reinterpret_cast<char*>(&crc16), 2); break;
            case 32: cookedData.append(reinterpret_cast<char*>(&crc), 4); break;
            default: break;
        }
    }

    emit invokeWriteBytes(cookedData);
}

void SAKDebuggerInput::changeInputModel(const QString &text)
{
    // Ignore empty text
    if (text.isEmpty()){
        return;
    }

    bool ok = false;
    mInputTextEdit->clear();
    mInputParameters.textFormat = mInputModelComboBox->currentData().toInt(&ok);
    mSettings->setValue(mSettingStringInputTextFromat, QVariant::fromValue(mInputModelComboBox->currentData().toInt()));
    Q_ASSERT_X(ok, __FUNCTION__, "Input model is error");
}

void SAKDebuggerInput::saveInputDataToFile()
{
    QString defaultName = QDateTime::currentDateTime().toString("yyyyMMdd").append(".txt");
    QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                    tr("Save File"),
                                                    QStandardPaths::writableLocation(QStandardPaths::DesktopLocation).append("/").append(defaultName),
                                                    tr("txt (*.txt);;all (*.*)"));

    if (fileName.isEmpty()){
        return;
    }

    QFile file(fileName);
    if (file.open(QFile::WriteOnly)){
        QByteArray data = mInputTextEdit->toPlainText().toUtf8();
        qint64 ret = file.write(data);
        if (ret == -1){
            //mDebugPage->outputMessage(file.errorString(), false);
        }
        file.close();
    }else {
        //mDebugPage->outputMessage(file.errorString(), false);
    }
}

void SAKDebuggerInput::readinFile()
{
    QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open File"));
    if (!fileName.isEmpty()){
        QFile file(fileName);
        if(file.open(QFile::ReadOnly)){
            QByteArray data = file.readAll();
            mInputTextEdit->setText(QString(data).toUtf8());
            file.close();
        }else{
            //mDebugPage->outputMessage(QString("%1:%2").arg(tr("Can not open file")).arg(fileName));
        }
    }
}

void SAKDebuggerInput::inputTextEditTextChanged()
{
    formattingInputText(mInputTextEdit, mInputParameters.textFormat);
    updateCrc();
}

void SAKDebuggerInput::sendRawData()
{
    QString data = mInputTextEdit->toPlainText();
    if (data.isEmpty()){
        auto ret = QMessageBox::warning(Q_NULLPTR, tr("Data is empty"), tr("Please input data then try again!"));
        Q_UNUSED(ret);
    }

    //emit rawDataChanged(data, mInputParameters);
}

void SAKDebuggerInput::changeCrcModel()
{
    bool ok = false;
    mInputParameters.crc.parametersModel = mCrcSettingsDialog->parametersContext().parameterMoldel;
    Q_ASSERT_X(ok, __FUNCTION__, "Please check the crc parameters model");

    updateCrc();
}

void SAKDebuggerInput::setPresetData()
{
//    if (mInputDataItemManager->isHidden()){
//        mInputDataItemManager->show();
//    }else{
//        mInputDataItemManager->activateWindow();
//    }
}

void SAKDebuggerInput::initParameters()
{
    mInputParameters.textFormat = mInputModelComboBox->currentData().toInt();

    SAKInputCrcSettingsDialog::SAKStructCrcParametersContext ctx = mCrcSettingsDialog->parametersContext();
    mInputParameters.crc.parametersModel = ctx.parameterMoldel;
    mInputParameters.crc.bigEndian = ctx.bigEndian;
    mInputParameters.crc.startByte = ctx.startByte;
    mInputParameters.crc.endByte = ctx.endByte;
    mInputParameters.crc.bigEndian = ctx.bigEndian;
}

void SAKDebuggerInput::cyclingWritingTimerTimeout()
{
    // If the mCyclingTimeComboBox is not enable, it means that the device is not ready.
    // So, do not write data to device.
    if (mCyclingTimeComboBox->isEnabled()){
        if (mInputTextEdit->toPlainText().length()){
            sendRawData();
        }
    }else{
        mCyclingTimeComboBox->setCurrentIndex(0);
        mCyclingWritingTimer.stop();
    }
}

void SAKDebuggerInput::updateCrc()
{
    QString rawData = mInputTextEdit->toPlainText();
    //QByteArray data = mInputDataFactory->rawDataToArray(rawData, mInputParameters);
    //QByteArray crcInputData = mInputDataFactory->extractCrcData(data, mInputParameters);

    //quint32 crc = mInputDataFactory->crcCalculate(crcInputData, mInputParameters.crc.parametersModel);
    //int bits =  mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(mInputParameters.crc.parametersModel));
    //mCrcLabel->setText(QString(QString("%1").arg(QString::number(crc, 16), (bits/8)*2, '0')).toUpper().prepend("0x"));
}

void SAKDebuggerInput::appendAction(SAKInputDataPresetItem *item)
{
    QString description = item->itemDescription();
    QAction *action = new QAction(description, mWriteDataItemMenu);
    action->setData(QVariant::fromValue(item));
    mWriteDataItemMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKDebuggerInput::actionTriggered);
}

void SAKDebuggerInput::removeAction(SAKInputDataPresetItem *item)
{
    QList<QAction*> actionList = mWriteDataItemMenu->actions();
    for (auto &var : actionList){
        if (var->data().value<SAKInputDataPresetItem*>() == item){
            var->deleteLater();
            break;
        }
    }
}

void SAKDebuggerInput::changeDescription(SAKInputDataPresetItem *item)
{
    QList<QAction*> actionList = mWriteDataItemMenu->actions();
    for (auto &var : actionList){
        if (var->data().value<SAKInputDataPresetItem*>() == item){
            var->setText(item->itemDescription());
            break;
        }
    }
}

void SAKDebuggerInput::actionTriggered()
{
    if (sender()){
        if (sender()->inherits("QAction")){
            QAction *action = qobject_cast<QAction*>(sender());
            SAKInputDataPresetItem *item = action->data().value<SAKInputDataPresetItem *>();
            //int format = item->itemTextFromat();
            QString text = item->itemText();

            if (mCyclingTimeComboBox->isEnabled()){
                //sendOtherRawData(text, format);
            }
        }
    }
}

void SAKDebuggerInput::readinSettings()
{
    QVariant var = mSettings->value(mSettingStringInputTextFromat);
    int index = 0;
    if (var.isNull()){
        index = 4;
    }else{
        index = var.toInt();
    }
    mInputModelComboBox->setCurrentIndex(index);
}
