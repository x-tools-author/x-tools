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
#include "SAKMainWindow.hh"
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
                                   QWidget *uiParent,
                                   QObject *parent)
    :QThread(parent)
    ,mRegularSendingComboBox(regularlySending)
    ,mTextFormatComboBox(inputFormat)
    ,mMoreInputSettingsPushButton(more)
    ,mSendPushButton(send)
    ,mInputComboBox(input)
    ,mCrcLabel(crc)
    ,mUiParent(uiParent)
    ,mDataPresetDialog(Q_NULLPTR)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
    ,mSqlDatabase(sqlDatabase)
    ,mRregularSendingTimer(new QTimer)
    ,mSuffixsActionGroup(Q_NULLPTR)
    ,mCrcInterface(new SAKCommonCrcInterface)
{
    // Initialize setting key.
    mSettingKeyCtx.suffixsType = settingsGroup + "/suffixsType";
    mSettingKeyCtx.inputTextFormat = settingsGroup + "/inputTextFormat";
    mSettingKeyCtx.enableSendingRecord = settingsGroup + "/enableSendingRecord";


    // Initialization
    initUi();
    initSubModule();


    // The thread will restart automatically after finishing,
    // if the data is not empty.
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
    if (isRunning()) {
        exit();
        wait();
    }
    mCrcInterface->deleteLater();
    mDataPreset->deleteLater();
    mCrcSettings->deleteLater();

    mRregularSendingTimer->stop();
    mRregularSendingTimer->deleteLater();

    mDataPresetDialog->close();
    mDataPresetDialog->deleteLater();
}

void SAKDebuggerInput::inputBytes(QString rawBytes,
                                  SAKStructInputParametersContext parasCtx)
{
    if (!rawBytes.isEmpty()) {
        mBytesInfoVectorMutex.lock();
        QPair<QString, SAKStructInputParametersContext> pair(rawBytes,
                                                             parasCtx);
        mBytesInfoVector.append(pair);
        if (!isRunning()) {
            start();
        }
        mBytesInfoVectorMutex.unlock();
    }
}

void SAKDebuggerInput::updateUiState(bool deviceIsOpened)
{
    mRegularSendingComboBox->setEnabled(deviceIsOpened);
    mSendPushButton->setEnabled(deviceIsOpened);
    mQuickSendingMenu->setEnabled(deviceIsOpened);

    if (!deviceIsOpened) {
        mRregularSendingTimer->stop();
        mRegularSendingComboBox->setCurrentIndex(0);
    }
}

void SAKDebuggerInput::run()
{
    auto takeBytesInfo =
            [&]()->QPair<QString, SAKDebuggerInput::SAKStructInputParametersContext> {
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
    if (rawData.isEmpty()) {
        return;
    }

    SAKStructInputParametersContext ctx = pair.second;
    auto textFormat =
            static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(ctx.textFormat);
    QByteArray cookedData =
            SAKCommonDataStructure::stringToByteArray(rawData, textFormat);


    if (ctx.suffixType != SAKCommonDataStructure::SuffixsTypeNone) {
        auto cookedSuffixType =
                static_cast<SAKCommonDataStructure::SAKEmnuSuffixsType>(ctx.suffixType);
        QString suffix = SAKCommonDataStructure::suffix(cookedSuffixType);
        cookedData.append(suffix.toLatin1());
    }


    if (ctx.crc.appending){
        QByteArray crcInputData = extractCrcData(cookedData, ctx);
        // Calculate the crc value of input data
        uint32_t crc  = crcCalculate(crcInputData, ctx.crc.parametersModel);
        uint8_t crc8  = static_cast<uint8_t>(crc);
        uint16_t crc16 = static_cast<uint16_t>(crc);
        int model = ctx.crc.parametersModel;
        auto cookedModel = static_cast<SAKCommonCrcInterface::SAKEnumCrcModel>(model);
        int bitsWidth = mCrcInterface->bitsWidth(cookedModel);
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

void SAKDebuggerInput::writeBytes()
{
    auto inputParametersTemp = mInputParameters;
    QString rawData = mInputComboBox->currentText();
    if (rawData.isEmpty()) {
        inputParametersTemp.textFormat = SAKCommonDataStructure::InputFormatAscii;
        rawData = QString("(empty)");
    }
    inputBytes(rawData, inputParametersTemp);
}

void SAKDebuggerInput::updateCrc()
{
    QString rawData = mInputComboBox->currentText();
    int format = mInputParameters.textFormat;
    auto parametersTemp = mInputParameters;
    auto cookedFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(
                format
                );
    QByteArray cookedData =
            SAKCommonDataStructure::stringToByteArray(rawData, cookedFormat);
    QByteArray crcInputData = extractCrcData(cookedData, parametersTemp);
    quint32 crc = crcCalculate(crcInputData, parametersTemp.crc.parametersModel);
    int bits =  mCrcInterface->bitsWidth(
                static_cast<SAKCommonCrcInterface::SAKEnumCrcModel>(
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

quint32 SAKDebuggerInput::crcCalculate(QByteArray data, int model)
{
    auto cookedModel = static_cast<SAKCommonCrcInterface::SAKEnumCrcModel>(model);
    int bitsWidth = mCrcInterface->bitsWidth(cookedModel);
    switch (bitsWidth) {
    case 8:
        return mCrcInterface->crcCalculate<uint8_t>(
                    reinterpret_cast<uint8_t*>(data.data()),
                    static_cast<quint64>(data.length()),
                    static_cast<SAKCommonCrcInterface::SAKEnumCrcModel>(model));
    case 16:
        return mCrcInterface->crcCalculate<uint16_t>(
                    reinterpret_cast<uint8_t*>(data.data()),
                    static_cast<quint64>(data.length()),
                    static_cast<SAKCommonCrcInterface::SAKEnumCrcModel>(model));
    case 32:
        return mCrcInterface->crcCalculate<uint32_t>(
                    reinterpret_cast<uint8_t*>(data.data()),
                    static_cast<quint64>(data.length()),
                    static_cast<SAKCommonCrcInterface::SAKEnumCrcModel>(model));
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

void SAKDebuggerInput::initUi()
{
    initUiRegularSendingComboBox();
    initUiTextFormatComboBox();
    initUiMoreInputSettingsPushButton();
    initUiSendPushButton();
    initUiInputComboBox();
}

void SAKDebuggerInput::initUiRegularSendingComboBox()
{
    mRegularSendingComboBox->addItem(tr("Forbidden"), INT_MAX);
    QString unit = tr("ms");
    for (int i = 50; i < 500; i += 50) {
        mRegularSendingComboBox->addItem(QString::number(i) + QString(" ") + unit, i);
    }
    for (int i = 1000; i <= 5000; i += 1000) {
        mRegularSendingComboBox->addItem(QString::number(i) + QString(" ") + unit, i);
    }
    connect(mRegularSendingComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [=](int index){
        mRregularSendingTimer->stop();
        if (index != 0) {
            int interval = mRegularSendingComboBox->currentData().toInt();
            mRregularSendingTimer->stop();
            mRregularSendingTimer->setInterval(interval);
            mRregularSendingTimer->start();
        }
    });
    connect(mRregularSendingTimer, &QTimer::timeout,
            this, &SAKDebuggerInput::writeBytes);
}

void SAKDebuggerInput::initUiTextFormatComboBox()
{
    // Input text format
    mTextFormatComboBox->blockSignals(true);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mTextFormatComboBox);
    QVariant textFormatVarant = mSettings->value(mSettingKeyCtx.inputTextFormat);
    int textFormat = textFormatVarant.toInt();
    QMetaEnum textFormatMetaEnum =
            QMetaEnum::fromType<SAKCommonDataStructure::SAKEnumTextFormatInput>();
    // If text format is unvalid, use the last value
    // of SAKCommonDataStructure::SAKEnumTextFormatInput
    if (!textFormatMetaEnum.valueToKey(textFormat)) {
        textFormat = textFormatMetaEnum.value(textFormatMetaEnum.keyCount() - 1);
    }
    auto cookedTextFormat =
            static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(textFormat);
    SAKCommonDataStructure::setLineEditTextFormat(mInputComboBox->lineEdit(),
                                                  cookedTextFormat);
    mInputParameters.textFormat = textFormat;
    for (int i = 0; i < mTextFormatComboBox->count(); i++){
        if (mTextFormatComboBox->itemData(i).toInt() == textFormat) {
            mTextFormatComboBox->setCurrentIndex(i);
            break;
        }
    }
    connect(mTextFormatComboBox, &QComboBox::currentTextChanged,
            this, [=](const QString &text){
        // Ignore empty text
        if (text.isEmpty()){
            return;
        }

        bool ok = false;
        mInputComboBox->lineEdit()->clear();
        mInputParameters.textFormat = mTextFormatComboBox->currentData().toInt(&ok);
        int model = mTextFormatComboBox->currentData().toInt();
        mSettings->setValue(this->mSettingKeyCtx.inputTextFormat, model);
        Q_ASSERT_X(ok, __FUNCTION__, "Input model is error");

        int format = mInputParameters.textFormat;
        auto cookedFormat =
                static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(format);
        SAKCommonDataStructure::setLineEditTextFormat(mInputComboBox->lineEdit(),
                                                      cookedFormat);
        updateCrc();
        mInputComboBox->clear();
    });
    mTextFormatComboBox->blockSignals(false);
}

void SAKDebuggerInput::initUiMoreInputSettingsPushButton()
{
    // Initialize more input settings button menu.
    mMoreInputSettingsPushButton->setToolTip(tr("More input settings"));
    auto moreInputSettingsPushButtonMenu = new QMenu(mMoreInputSettingsPushButton);
    mMoreInputSettingsPushButton->setMenu(moreInputSettingsPushButtonMenu);


    addActionToMenuQuickSending(moreInputSettingsPushButtonMenu);
    addActionToMenuDataPreset(moreInputSettingsPushButtonMenu);
    auto ret = moreInputSettingsPushButtonMenu->addSeparator();
    Q_UNUSED(ret);
    addActionToMenuSuffixs(moreInputSettingsPushButtonMenu);
    addActionToMenuSaveInput(moreInputSettingsPushButtonMenu);
    addActionToMenuClearInput(moreInputSettingsPushButtonMenu);
    addActionToMenuCRCSettings(moreInputSettingsPushButtonMenu);
    addActionToMenuEnableSendingRecord(moreInputSettingsPushButtonMenu);
}

void SAKDebuggerInput::initUiSendPushButton()
{
    connect(mSendPushButton, &QPushButton::clicked,
            this, [=](){
        writeBytes();

        QString data = mInputComboBox->currentText();
        if (data.isEmpty()) {
            return;
        }

        if (data.toUtf8().size() > 256) {
            return;
        }

        bool isExit = false;
        for (int i = 0; i < mInputComboBox->count(); i++) {
            if (mInputComboBox->currentText() == mInputComboBox->itemText(i)) {
                isExit = true;
                break;
            }
        }

        if (!isExit) {
            mInputComboBox->addItem(mInputComboBox->currentText());
        }

        if (mInputComboBox->count() > 10) {
            mInputComboBox->removeItem(0);
        };
    });
}

void SAKDebuggerInput::initUiInputComboBox()
{
    connect(mInputComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, &SAKDebuggerInput::updateCrc);
}

void SAKDebuggerInput::addActionToMenuQuickSending(QMenu *menu)
{
    mQuickSendingMenu = new QMenu(tr("Quick Sending"));
    menu->addMenu(mQuickSendingMenu);
}

void SAKDebuggerInput::addActionToMenuDataPreset(QMenu *menu)
{
    menu->addAction(tr("Data Preset"), this, [=](){
        if (mDataPresetDialog->isHidden()) {
            mDataPresetDialog->show();
        } else {
            mDataPresetDialog->activateWindow();
        }
    });
}

void SAKDebuggerInput::addActionToMenuSuffixs(QMenu *menu)
{
    QMenu *suffixsMenu = menu->addMenu(tr("Suffixs"));
    QMetaEnum suffixsMetaEnum =
            QMetaEnum::fromType<SAKCommonDataStructure::SAKEmnuSuffixsType>();
    QVariant suffixsTypeVariant = mSettings->value(mSettingKeyCtx.suffixsType);
    int suffixsType = suffixsTypeVariant.toInt();
    mInputParameters.suffixType = suffixsType;
    mSuffixsActionGroup = new QActionGroup(this);
    for (int i = 0; i < suffixsMetaEnum.keyCount(); i++) {
        int type = suffixsMetaEnum.keyToValue(suffixsMetaEnum.key(i));
        auto cookedType = static_cast<SAKCommonDataStructure::SAKEmnuSuffixsType>(type);
        QString friendlySuffix = SAKCommonDataStructure::friendlySuffix(cookedType);
        if (friendlySuffix.isEmpty()) {
            friendlySuffix = tr("(None)");
        }
        QAction *suffixAction = suffixsMenu->addAction(friendlySuffix, this, [=](){
            mSettings->setValue(mSettingKeyCtx.suffixsType, cookedType);
            mInputParameters.suffixType = cookedType;
        });
        suffixAction->setCheckable(true);
        mSuffixsActionGroup->addAction(suffixAction);
        if (suffixsType == cookedType) {
            suffixAction->setChecked(true);
        }
    }
}

void SAKDebuggerInput::addActionToMenuSaveInput(QMenu *menu)
{
    menu->addAction(tr("Save Input"), this, [=](){
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
}

void SAKDebuggerInput::addActionToMenuClearInput(QMenu *menu)
{
    menu->addAction(tr("Clear Input"), this, [=](){
        mInputComboBox->lineEdit()->clear();
    });
}

void SAKDebuggerInput::addActionToMenuCRCSettings(QMenu *menu)
{
    menu->addAction(tr("CRC Settings"), this, [=](){
        if (mCrcSettings->isHidden()) {
            mCrcSettings->show();
        } else {
            mCrcSettings->activateWindow();
        }
    });
}

void SAKDebuggerInput::addActionToMenuEnableSendingRecord(QMenu *menu)
{
    auto *action = menu->addAction(
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
            mInputComboBox->clear();
        }
    });
}

void SAKDebuggerInput::initSubModule()
{
    initSubModuleDataPreset();
    initSubModuleCrcSettings();
}

void SAKDebuggerInput::initSubModuleDataPreset()
{
    mDataPresetDialog = new QDialog(sakMainWindow);
    QHBoxLayout *layout = new QHBoxLayout();
    layout->setContentsMargins(0, 0, 0, 0);
    mDataPresetDialog->setLayout(layout);
    mDataPresetDialog->setModal(true);
    mDataPresetDialog->setContentsMargins(2, 2, 2, 2);
    mDataPresetDialog->resize(800, 350);
    mDataPresetDialog->setWindowTitle(tr("Preset Data"));


    mDataPreset = new SAKDebuggerInputDataPreset(mSqlDatabase,
                                                 mSettings,
                                                 mSettingsGroup,
                                                 mQuickSendingMenu,
                                                 mDataPresetDialog);
    mDataPresetDialog->layout()->addWidget(mDataPreset);
    connect(mDataPreset, &SAKDebuggerInputDataPreset::invokeWriteString,
            this, [&](QString rawData, int format){
        auto parasCtx = mInputParameters;
        parasCtx.textFormat = format;
        inputBytes(rawData, parasCtx);
    });
}

void SAKDebuggerInput::initSubModuleCrcSettings()
{
    auto updateCrcParameters = [&](){
        auto ctx = mCrcSettings->parametersContext();
        mInputParameters.crc.parametersModel = ctx.parameterMoldel;
        mInputParameters.crc.appending = ctx.append;
        mInputParameters.crc.bigEndian = ctx.bigEndian;
        mInputParameters.crc.startByte = ctx.startByte;
        mInputParameters.crc.endByte = ctx.endByte;
        updateCrc();
    };
    mCrcSettings = new SAKDebuggerInputCrcSettings(mSettingsGroup, mSettings, mUiParent);
    updateCrcParameters();
    connect(mCrcSettings, &SAKDebuggerInputCrcSettings::crcParametersChanged,
            this, updateCrcParameters);
}
