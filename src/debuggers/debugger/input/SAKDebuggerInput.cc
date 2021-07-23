/****************************************************************************************
 * Copyright 2020-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMenu>
#include <QFile>
#include <QDebug>
#include <QAction>
#include <QtEndian>
#include <QLineEdit>
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKDebugger.hh"
#include "SAKDebuggerInput.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerInputDataPreset.hh"
#include "SAKDebuggerInputCrcSettings.hh"

SAKDebuggerInput::SAKDebuggerInput(QComboBox *regularlySending,
                                   QComboBox *inputFormat,
                                   QPushButton *more,
                                   QPushButton *send,
                                   QLabel *crc,
                                   QComboBox *input,
                                   QSettings *settings,
                                   const QString &settingsGroup,
                                   QSqlDatabase *sqlDatabase,
                                   QObject *parent)
    :QThread(parent)
    ,mCyclingTimeComboBox(regularlySending)
    ,mInputModelComboBox(inputFormat)
    ,mMoreInputSettingsPushButton(more)
    ,mSendPushButton(send)
    ,mInputComboBox(input)
    ,mCrcLabel(crc)
    ,mCrcInterface(new SAKCommonCrcInterface)
    ,mSettings(settings)
{
    // Initialize setting key
    mSettingKeyCtx.suffixsType = settingsGroup + "suffixsType";
    mSettingKeyCtx.inputTextFormat = settingsGroup + "inputTextFormat";
    mSettingKeyCtx.enableSendingRecord = settingsGroup + "enableSendingRecord";


    mCyclingTimeComboBox->addItem(tr("Forbidden"), INT_MAX);
    QString unit = tr("ms");
    for (int i = 50; i < 500; i += 50) {
        mCyclingTimeComboBox->addItem(QString::number(i) + QString(" ") + unit, i);
    }
    for (int i = 1000; i <= 5000; i += 1000) {
        mCyclingTimeComboBox->addItem(QString::number(i) + QString(" ") + unit, i);
    }
    connect(mCyclingTimeComboBox,
            &QComboBox::currentTextChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        if (mCyclingTimeComboBox->currentIndex() != 0) {
            int interval = mCyclingTimeComboBox->currentData().toInt();
            mCyclingWritingTimer.setInterval(interval);
            mCyclingWritingTimer.start();
        } else {
            mCyclingWritingTimer.stop();
        }
    });


    // Initialize more input settings button menu
    mMoreInputSettingsPushButton->setToolTip(tr("More input settings"));
    auto moreInputSettingsPushButtonMenu = new QMenu;
    connect(this, &QObject::destroyed,
            moreInputSettingsPushButtonMenu, &QMenu::deleteLater);
    mMoreInputSettingsPushButton->setMenu(moreInputSettingsPushButtonMenu);


    mQuickSendingMenu = new QMenu(tr("Quick Sending"));
    moreInputSettingsPushButtonMenu->addMenu(mQuickSendingMenu);
    moreInputSettingsPushButtonMenu->addAction(tr("Set Data Items"), this, [=](){
        if (mInputDataItemManager->isHidden()) {
            mInputDataItemManager->show();
        } else {
            mInputDataItemManager->activateWindow();
        }
    });
    auto ret = moreInputSettingsPushButtonMenu->addSeparator();
    Q_UNUSED(ret);


    QMenu *suffixsMenu = moreInputSettingsPushButtonMenu->addMenu(tr("Suffixs"));
    QMetaEnum suffixsMetaEnum =
            QMetaEnum::fromType<SAKCommonDataStructure::SAKEmnuSuffixsType>();
    QVariant suffixsTypeVariant = mSettings->value(mSettingKeyCtx.suffixsType);
    int suffixsType = suffixsTypeVariant.toInt();
    QActionGroup *suffixsActionGroup = new QActionGroup(this);
    for (int i = 0; i < suffixsMetaEnum.keyCount(); i++) {
        int type = suffixsMetaEnum.keyToValue(suffixsMetaEnum.key(i));
        auto cookedType = static_cast<SAKCommonDataStructure::SAKEmnuSuffixsType>(type);
        QString friendlySuffix = SAKCommonDataStructure::friendlySuffix(cookedType);
        if (friendlySuffix.isEmpty()) {
            friendlySuffix = tr("(None)");
        }
        QAction *suffixAction = suffixsMenu->addAction(friendlySuffix, this, [=](){
            mSettings->setValue(mSettingKeyCtx.suffixsType, cookedType);
            mInputParameters.suffixsType = cookedType;
        });
        suffixAction->setCheckable(true);
        suffixsActionGroup->addAction(suffixAction);
        if (suffixsType == cookedType) {
            suffixAction->setChecked(true);
        }
    }


    moreInputSettingsPushButtonMenu->addAction(tr("Save Input"), this, [=](){
        QDateTime dateTime = QDateTime::currentDateTime();
        QString defaultName = dateTime.toString("yyyyMMdd").append(".txt");
        auto location = QStandardPaths::DesktopLocation;
        QString defaulePath = QStandardPaths::writableLocation(location);
        defaulePath.append("/").append(defaultName);
        QString fileName = QFileDialog::getSaveFileName(Q_NULLPTR,
                                                        tr("Save File"),
                                                        defaulePath,
                                                        tr("txt (*.txt);;all (*.*)"));
        if (fileName.isEmpty()){
            return;
        }

        QFile file(fileName);
        if (file.open(QFile::WriteOnly)){
            QByteArray data = mInputComboBox->currentText().toUtf8();
            qint64 ret = file.write(data);
            if (ret == -1) {
                emit messageChanged(file.errorString(), true);
            }
            file.close();
        }else {
            emit messageChanged(file.errorString(), true);
        }
    });


    moreInputSettingsPushButtonMenu->addAction(tr("Clear Input"), this, [=](){
        mInputComboBox->lineEdit()->clear();
    });


#if 0
    moreInputSettingsPushButtonMenu->addAction(tr("Readin File"), this, [=](){
        QString fileName = QFileDialog::getOpenFileName(Q_NULLPTR, tr("Open File"));
        if (!fileName.isEmpty()){
            QFile file(fileName);
            if(file.open(QFile::ReadOnly)){
                QByteArray data = file.readAll();
                mInputComboBox->setCurrentText(QString(data).toUtf8());
                file.close();
            }else{
                QString msg = QString("%1:%2").arg(tr("Can not open file"), fileName);
                emit messageChanged(msg, false);
            }
        }
    });
#endif


    moreInputSettingsPushButtonMenu->addAction(tr("CRC Settings"), this, [=](){
        if (mCrcSettings->isHidden()) {
            mCrcSettings->show();
        } else {
            mCrcSettings->activateWindow();
        }
    });


    auto *action = moreInputSettingsPushButtonMenu->addAction(
                tr("Enable Sending Record")
                );
    action->setCheckable(true);
    auto valueVariant = mSettings->value(mSettingKeyCtx.enableSendingRecord);
    if (valueVariant.isNull()) {
        mEnableSendingRecord = true;
        action->setChecked(mEnableSendingRecord);
    } else {
        auto checked = valueVariant.toBool();
        mEnableSendingRecord = checked;
        action->setChecked(checked);
    }
    connect(action, &QAction::triggered, this, [=](){
        mSettings->setValue(mSettingKeyCtx.enableSendingRecord, action->isChecked());
        this->mEnableSendingRecord = action->isChecked();
        if (!this->mEnableSendingRecord) {
            input->clear();
        }
    });


    // Initializing variables about settings
    QString group = settingsGroup;
    mSettings = settings;
    Q_ASSERT_X(mSettings, __FUNCTION__, "Value can not be null!");
    mSettingStringInputTextFromat = QString("%1/inputTextFormat").arg(group);

    // InputParametersContext will be a parameter of signal,
    // so, do something make compiling happy
    qRegisterMetaType<SAKStructInputParametersContext>("InputParameters");

    // Add actions after new.
    mInputDataItemManager = new SAKDebuggerInputDataPreset(sqlDatabase,
                                                           settings,
                                                           settingsGroup,
                                                           mQuickSendingMenu);
    connect(mInputDataItemManager, &SAKDebuggerInputDataPreset::invokeWriteBytes,
            this, [&](QString rawData, int format){
        mInputParametersMutex.lock();
        auto parasCtx = mInputParameters;
        mInputParametersMutex.unlock();
        parasCtx.textFormat = format;
        inputBytes(rawData, parasCtx);
    });


    mCrcSettings = new SAKDebuggerInputCrcSettings(settingsGroup, settings);
    auto ctx = mCrcSettings->parametersContext();
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


    // Update parameters
    connect(mCrcSettings, &SAKDebuggerInputCrcSettings::crcParametersChanged,
            this, [&](){
        auto ctx = mCrcSettings->parametersContext();
        mInputParameters.crc.parametersModel = ctx.parameterMoldel;
        mInputParameters.crc.appending = ctx.append;
        mInputParameters.crc.bigEndian = ctx.bigEndian;
        mInputParameters.crc.startByte = ctx.startByte;
        mInputParameters.crc.endByte = ctx.endByte;

        updateCrc();
    });


    connect(mInputModelComboBox, &QComboBox::currentTextChanged,
            this, &SAKDebuggerInput::changeInputModel);
    connect(mSendPushButton, &QPushButton::clicked,
            this, &SAKDebuggerInput::sendRawData);
    connect(mInputComboBox, &QComboBox::currentTextChanged,
            this, &SAKDebuggerInput::inputTextEditTextChanged);
    connect(&mCyclingWritingTimer, &QTimer::timeout,
            this, &SAKDebuggerInput::cyclingWritingTimerTimeout);


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
    mCrcInterface->deleteLater();
    mInputDataItemManager->deleteLater();
    mCrcSettings->deleteLater();
}

void SAKDebuggerInput::showCrcSettingsDialog()
{
    if (mCrcSettings->isHidden()) {
        mCrcSettings->show();
    } else {
        mCrcSettings->activateWindow();
    }
}

void SAKDebuggerInput::formattingInputText(QTextEdit *textEdit, int model)
{
    if (!textEdit){
        return;
    }

    textEdit->blockSignals(true);
    QString plaintext = textEdit->toPlainText();
    if (!plaintext.isEmpty()){
        QString cookedString = SAKCommonDataStructure::formattingString(
                    plaintext,
                    static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(model)
                    );
        textEdit->setText(cookedString);
        textEdit->moveCursor(QTextCursor::End);
    }
    textEdit->blockSignals(false);
}

void SAKDebuggerInput::inputBytes(QString rawBytes,
                                  SAKStructInputParametersContext parametersContext)
{
    if (!rawBytes.isEmpty()) {
        mBytesInfoVectorMutex.lock();
        QPair<QString, SAKStructInputParametersContext> pair(rawBytes,
                                                             parametersContext);
        mBytesInfoVector.append(pair);
        if (!isRunning()) {
            start();
        }
        mBytesInfoVectorMutex.unlock();
    }
}

void SAKDebuggerInput::run()
{
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
    auto textFormat =
            static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(ctx.textFormat);
    QByteArray cookedData =
            SAKCommonDataStructure::stringToByteArray(rawData, textFormat);


    if (ctx.crc.appending){
        QByteArray crcInputData = extractCrcData(cookedData, ctx);
        // Calculate the crc value of input data
        uint32_t crc  = crcCalculate(crcInputData, ctx.crc.parametersModel);
        uint8_t crc8  = static_cast<uint8_t>(crc);
        uint16_t crc16 = static_cast<uint16_t>(crc);
        int model = ctx.crc.parametersModel;
        auto cookedModel = static_cast<SAKCommonCrcInterface::CRCModel>(model);
        int bitsWidth = mCrcInterface->getBitsWidth(cookedModel);
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
    mInputComboBox->lineEdit()->clear();
    mInputParameters.textFormat = mInputModelComboBox->currentData().toInt(&ok);
    mSettings->setValue(mSettingStringInputTextFromat,
                        QVariant::fromValue(mInputModelComboBox->currentData().toInt()));
    Q_ASSERT_X(ok, __FUNCTION__, "Input model is error");

    int format = mInputParameters.textFormat;
    auto cookedFormat = static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(format);
    SAKCommonDataStructure::setLineEditTextFormat(mInputComboBox->lineEdit(), cookedFormat);
}

void SAKDebuggerInput::inputTextEditTextChanged()
{
    //formattingInputText(mInputTextEdit, mInputParameters.textFormat);
    updateCrc();
}

void SAKDebuggerInput::sendRawData()
{
    QString data = mInputComboBox->currentText();
    if (data.isEmpty()){
        auto ret = QMessageBox::warning(Q_NULLPTR,
                                        tr("Data is empty"),
                                        tr("Please input data then try again!"));
        Q_UNUSED(ret);
    }

    //emit rawDataChanged(data, mInputParameters);
}

void SAKDebuggerInput::changeCrcModel()
{
    bool ok = false;
    auto parametersCtx = mCrcSettings->parametersContext();
    mInputParameters.crc.parametersModel = parametersCtx.parameterMoldel;
    Q_ASSERT_X(ok, __FUNCTION__, "Please check the crc parameters model");

    updateCrc();
}

void SAKDebuggerInput::initParameters()
{
    mInputParameters.textFormat = mInputModelComboBox->currentData().toInt();

    auto ctx = mCrcSettings->parametersContext();
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
        if (mInputComboBox->currentText().length()){
            sendRawData();
        }
    }else{
        mCyclingTimeComboBox->setCurrentIndex(0);
        mCyclingWritingTimer.stop();
    }
}

void SAKDebuggerInput::updateCrc()
{
    QString rawData = mInputComboBox->currentText();
    mInputParametersMutex.lock();
    int format = mInputParameters.textFormat;
    auto parametersTemp = mInputParameters;
    mInputParametersMutex.unlock();
    auto cookedFormat = static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(
                format
                );
    QByteArray cookedData =
            SAKCommonDataStructure::stringToByteArray(rawData, cookedFormat);
    QByteArray crcInputData = extractCrcData(cookedData, parametersTemp);
    quint32 crc = crcCalculate(crcInputData, parametersTemp.crc.parametersModel);
    int bits =  mCrcInterface->getBitsWidth(
                static_cast<SAKCommonCrcInterface::CRCModel>(
                    mInputParameters.crc.parametersModel
                    )
                );
    int fillWidth = bits/8*2;
    QString crcString = QString::number(crc, 16);
    crcString = QString("%1").arg(crcString, fillWidth, '0');
    crcString = crcString.toUpper();
    crcString.prepend("0x");
    mCrcLabel->setText(crcString);
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

quint32 SAKDebuggerInput::crcCalculate(QByteArray data, int model)
{
    auto cookedModel = static_cast<SAKCommonCrcInterface::CRCModel>(model);
    int bitsWidth = mCrcInterface->getBitsWidth(cookedModel);
    switch (bitsWidth) {
    case 8:
        return mCrcInterface->crcCalculate<uint8_t>(
                    reinterpret_cast<uint8_t*>(data.data()),
                    static_cast<quint64>(data.length()),
                    static_cast<SAKCommonCrcInterface::CRCModel>(model));
    case 16:
        return mCrcInterface->crcCalculate<uint16_t>(
                    reinterpret_cast<uint8_t*>(data.data()),
                    static_cast<quint64>(data.length()),
                    static_cast<SAKCommonCrcInterface::CRCModel>(model));
    case 32:
        return mCrcInterface->crcCalculate<uint32_t>(
                    reinterpret_cast<uint8_t*>(data.data()),
                    static_cast<quint64>(data.length()),
                    static_cast<SAKCommonCrcInterface::CRCModel>(model));
    default:
        return 0;
    }
}

QByteArray SAKDebuggerInput::extractCrcData(QByteArray crcData,
                          SAKStructInputParametersContext parameters)
{
    QByteArray crcInputData;
    int startIndex = parameters.crc.startByte - 1;
    startIndex = startIndex < 0 ? 0 : startIndex;

    int endIndex = (crcData.length() - 1) - (parameters.crc.endByte - 1);
    endIndex = endIndex < 0 ? 0 : endIndex;

    if (((crcData.length() - 1) >= startIndex)
            && ((crcData.length() - 1) >= endIndex)){
        int length = endIndex - startIndex + 1;
        length = length < 0 ? 0 : length;
        crcInputData = QByteArray(crcData.constData()+startIndex, length);
    }else{
        crcInputData = crcData;
    }
    return crcInputData;
}
