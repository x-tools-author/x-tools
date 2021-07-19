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
#include <QMessageBox>
#include <QFileDialog>
#include <QStandardPaths>
#include <QListWidgetItem>

#include "SAKDebugger.hh"
#include "SAKDebuggerInput.hh"
#include "SAKInputDataFactory.hh"
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
    qRegisterMetaType<InputParametersContext>("InputParameters");
    mInputDataFactory = new SAKInputDataFactory;
    mInputDataFactory->start();

    // Add actions after new.
#if 0
    mInputDataItemManager = new SAKInputDataPresetItemManager(debugPage);
    QList<SAKInputDataPresetItem*> list = mInputDataItemManager->itemList();
    for (auto &var : list){
        appendAction(var);
    }
#endif

    mCrcSettingsDialog = new SAKInputCrcSettingsDialog(settingsGroup, settings);
    SAKInputCrcSettingsDialog::CrcParameterContext ctx = mCrcSettingsDialog->parametersContext();
    mInputParameters.crcParametersModel = ctx.crcPrameterMoldel;
    mInputParameters.appendCrc = ctx.appendCrc;
    mInputParameters.bigEndian = ctx.bigEndianCrc;
    mInputParameters.startByte = ctx.startByte;
    mInputParameters.endByte = ctx.endByte;

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
        SAKInputCrcSettingsDialog::CrcParameterContext ctx = mCrcSettingsDialog->parametersContext();
        mInputParameters.crcParametersModel = ctx.crcPrameterMoldel;
        mInputParameters.appendCrc = ctx.appendCrc;
        mInputParameters.bigEndian = ctx.bigEndianCrc;
        mInputParameters.startByte = ctx.startByte;
        mInputParameters.endByte = ctx.endByte;

        updateCrc();
    });

    connect(mInputModelComboBox, &QComboBox::currentTextChanged, this, &SAKDebuggerInput::changeInputModel);
    connect(mSendPushButton, &QPushButton::clicked, this, &SAKDebuggerInput::sendRawData);
    connect(mInputTextEdit, &QTextEdit::textChanged, this, &SAKDebuggerInput::inputTextEditTextChanged);
    connect(this, &SAKDebuggerInput::rawDataChanged, mInputDataFactory, &SAKInputDataFactory::cookData);
    //connect(mInputDataFactory, &SAKInputDataFactory::dataCooked, debugPage, &SAKDebugger::write);
    connect(&mCyclingWritingTimer, &QTimer::timeout, this, &SAKDebuggerInput::cyclingWritingTimerTimeout);
    //connect(debugPage, &SAKDebugger::requestWriteRawData, this, &SAKDebuggerInput::sendOtherRawData);

    initParameters();
    updateCrc();
}

SAKDebuggerInput::~SAKDebuggerInput()
{
    delete mInputDataFactory;
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

void SAKDebuggerInput::changeInputModel(const QString &text)
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
    formattingInputText(mInputTextEdit, mInputParameters.inputModel);
    updateCrc();
}

void SAKDebuggerInput::sendRawData()
{
    QString data = mInputTextEdit->toPlainText();
    if (data.isEmpty()){
        auto ret = QMessageBox::warning(Q_NULLPTR, tr("Data is empty"), tr("Please input data then try again!"));
        Q_UNUSED(ret);
    }

    emit rawDataChanged(data, mInputParameters);
}

void SAKDebuggerInput::sendOtherRawData(QString data, int textFormat)
{
    InputParametersContext temp = mInputParameters;
    temp.inputModel = textFormat;

    emit rawDataChanged(data, temp);
}

void SAKDebuggerInput::changeCrcModel()
{
    bool ok = false;
    mInputParameters.crcParametersModel = mCrcSettingsDialog->parametersContext().crcPrameterMoldel;
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
    mInputParameters.inputModel = mInputModelComboBox->currentData().toInt();

    SAKInputCrcSettingsDialog::CrcParameterContext ctx = mCrcSettingsDialog->parametersContext();
    mInputParameters.crcParametersModel = ctx.crcPrameterMoldel;
    mInputParameters.bigEndian = ctx.bigEndianCrc;
    mInputParameters.startByte = ctx.startByte;
    mInputParameters.endByte = ctx.endByte;
    mInputParameters.bigEndian = ctx.bigEndianCrc;
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
    QByteArray data = mInputDataFactory->rawDataToArray(rawData, mInputParameters);
    QByteArray crcInputData = mInputDataFactory->extractCrcData(data, mInputParameters);

    quint32 crc = mInputDataFactory->crcCalculate(crcInputData, mInputParameters.crcParametersModel);
    int bits =  mCrcInterface->getBitsWidth(static_cast<SAKCommonCrcInterface::CRCModel>(mInputParameters.crcParametersModel));
    mCrcLabel->setText(QString(QString("%1").arg(QString::number(crc, 16), (bits/8)*2, '0')).toUpper().prepend("0x"));
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
            int format = item->itemTextFromat();
            QString text = item->itemText();

            if (mCyclingTimeComboBox->isEnabled()){
                sendOtherRawData(text, format);
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
