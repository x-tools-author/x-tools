/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "SAKDebugger.hh"
#include "SAKCommonInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginAutoResponseItem.hh"
#include "ui_SAKDebuggerPluginAutoResponseItem.h"

SAKDebuggerPluginAutoResponseItem::SAKDebuggerPluginAutoResponseItem(QWidget *parent)
    :SAKBaseListWidgetItemWidget(parent)
    ,mUi(new Ui::SAKDebuggerPluginAutoResponseItem)
{
    mUi->setupUi(this);
    blockUiComponentsSignals(true);
    setupItem();
    blockUiComponentsSignals(false);
}

SAKDebuggerPluginAutoResponseItem::SAKDebuggerPluginAutoResponseItem(
        SAKStructItemContext ctx,
        QWidget *parent)
    :SAKBaseListWidgetItemWidget(ctx.id, parent)
    ,mUi(new Ui::SAKDebuggerPluginAutoResponseItem)
{
    mUi->setupUi(this);
    blockUiComponentsSignals(true);


    setupItem();
    mUi->descriptionLineEdit->setText(ctx.description);
    mUi->referenceDataLineEdit->setText(ctx.referenceData);
    mUi->responseDataLineEdit->setText(ctx.responseData);
    mUi->enableCheckBox->setChecked(ctx.enable);
    mUi->referenceDataFromatComboBox->setCurrentIndex(ctx.referenceFormat);
    mUi->responseDataFormatComboBox->setCurrentIndex(ctx.responseFormat);
    mUi->optionComboBox->setCurrentIndex(ctx.option);
    mUi->enableDelayCheckBox->setChecked(ctx.enableDelay);
    mUi->delayTimeSpinBox->setValue(ctx.delayTime);

    SAKCommonDataStructure::setLineEditTextFormat(mUi->referenceDataLineEdit,
                                                  ctx.referenceFormat);
    SAKCommonDataStructure::setLineEditTextFormat(mUi->responseDataLineEdit,
                                                  ctx.responseFormat);

    blockUiComponentsSignals(false);
}

SAKDebuggerPluginAutoResponseItem::~SAKDebuggerPluginAutoResponseItem()
{
    delete mUi;
}

SAKDebuggerPluginAutoResponseItem::SAKStructItemContext
SAKDebuggerPluginAutoResponseItem::context()
{
    SAKStructItemContext ctx;
    ctx.id = id();
    ctx.description = mUi->descriptionLineEdit->text();
    ctx.enableDelay = mUi->enableDelayCheckBox->isChecked();
    ctx.enable = mUi->enableCheckBox->isChecked();
    ctx.option = mUi->optionComboBox->currentIndex();
    ctx.delayTime = mUi->delayTimeSpinBox->value();
    ctx.referenceData = mUi->referenceDataLineEdit->text();
    ctx.responseData = mUi->responseDataLineEdit->text();
    ctx.responseFormat = mUi->responseDataFormatComboBox->currentData().toInt();
    ctx.referenceFormat = mUi->referenceDataFromatComboBox->currentData().toInt();

    return ctx;
}

void SAKDebuggerPluginAutoResponseItem::onBytesRead(QByteArray bytes)
{
    if (bytes.isEmpty() || (!mUi->enableCheckBox->isChecked())) {
        return;
    }

    QString referenceString;
    int referenceFormat = mUi->referenceDataFromatComboBox->currentData().toInt();
    if ((referenceFormat == SAKCommonDataStructure::InputFormatBin) ||
            (referenceFormat == SAKCommonDataStructure::InputFormatOct) ||
            (referenceFormat == SAKCommonDataStructure::InputFormatDec) ||
            (referenceFormat == SAKCommonDataStructure::InputFormatHex)) {
        referenceString = mUi->referenceDataLineEdit->text().trimmed();
    } else {
        referenceString = mUi->referenceDataLineEdit->text();
    }


    auto cookedReferenceFormat =
            static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(referenceFormat);
    QByteArray referenceData =
            SAKCommonDataStructure::stringToByteArray(referenceString,
                                                      cookedReferenceFormat);
    if (needToResponse(bytes, referenceData,
                       mUi->optionComboBox->currentData().toInt())) {
         QString responseString = mUi->responseDataLineEdit->text();
         int responseFromat = mUi->referenceDataFromatComboBox->currentData().toInt();
         auto cookedResponseFromat =
                 static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(
                     responseFromat
                     );
         QByteArray responseData = SAKCommonDataStructure::stringToByteArray(
                     responseString,
                     cookedResponseFromat
                     );


         if (!responseData.isEmpty()) {
             if (mUi->enableDelayCheckBox->isChecked()) {
                 int delay = mUi->delayTimeSpinBox->value();
                 QTimer::singleShot(delay, this, [=](){
                     emit invokeWriteCookedBytes(responseData);
                 });
             } else {
                 emit invokeWriteCookedBytes(responseData);
             }
         }
    }
}

void SAKDebuggerPluginAutoResponseItem::setupItem()
{
    mUi->optionComboBox->clear();
    int dataValue = ReadDataIsEqualToReference;
    mUi->optionComboBox->addItem(tr("Rx data is equal to reference data"),
                                 QVariant::fromValue<int>(dataValue));
    dataValue = ReadDataContainsReferenceData;
    mUi->optionComboBox->addItem(tr("Rx data contains reference data"),
                                 QVariant::fromValue<int>(dataValue));
    dataValue = ReadDataDoesNotContainReferenceData;
    mUi->optionComboBox->addItem(tr("Rx data does not contains reference data"),
                                 QVariant::fromValue<int>(dataValue));

    SAKCommonDataStructure::setComboBoxTextInputFormat(
                mUi->referenceDataFromatComboBox
                );
    SAKCommonDataStructure::setComboBoxTextInputFormat(
                mUi->responseDataFormatComboBox
                );

    connect(mUi->descriptionLineEdit, &QLineEdit::textChanged,
            this, [&](const QString description){
        emit descriptionChanged(id(), description);
    });

    connect(mUi->enableCheckBox, &QCheckBox::clicked,
            this, [&](){
        bool enable = mUi->enableCheckBox->isChecked();
        setEnable(enable);
        emit enableChanged(id(), enable);
    });

    connect(mUi->referenceDataLineEdit, &QLineEdit::textChanged,
            this, [&](const QString description){
        emit referenceTextChanged(id(), description);
    });

    connect(mUi->responseDataLineEdit, &QLineEdit::textChanged,
            this, [&](const QString description){
        emit responseTextChanged(id(), description);
    });


    connect(mUi->optionComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [&](int option){
        emit optionChanged(id(), option);
    });


    connect(mUi->referenceDataFromatComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [&](int format){
        emit referenceFormatChanged(id(), format);

        mUi->referenceDataLineEdit->clear();
        SAKCommonDataStructure::setLineEditTextFormat(mUi->referenceDataLineEdit, format);
    });

    connect(mUi->responseDataFormatComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this, [&](int format){
        emit responseFromatChanged(id(), format);

        mUi->responseDataLineEdit->clear();
        SAKCommonDataStructure::setLineEditTextFormat(mUi->responseDataLineEdit, format);
    });


    connect(mUi->enableDelayCheckBox, &QCheckBox::clicked,
            this, [&](){
        emit enableDelayChanged(id(), mUi->enableDelayCheckBox->isChecked());
    });


    connect(mUi->delayTimeSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [&](int interval){
        emit delayTimeChanged(id(), interval);
    });

    setEnable(mUi->enableCheckBox->isChecked());
}

bool SAKDebuggerPluginAutoResponseItem::needToResponse(QByteArray receiveData,
                                                       QByteArray referenceData,
                                                       int option)
{
    if (option == ReadDataIsEqualToReference){
        return (QString(receiveData.toHex()) == QString(referenceData.toHex()));
    }

    if (option == ReadDataContainsReferenceData){
        return (QString(receiveData.toHex()).contains(QString(referenceData.toHex())));
    }

    if (option == ReadDataDoesNotContainReferenceData){
        return !(QString(receiveData.toHex()).contains(QString(referenceData.toHex())));
    }

    return false;
}

void SAKDebuggerPluginAutoResponseItem::blockUiComponentsSignals(bool block)
{
    mUi->descriptionLineEdit->blockSignals(block);
    mUi->referenceDataLineEdit->blockSignals(block);
    mUi->responseDataLineEdit->blockSignals(block);
    mUi->optionComboBox->blockSignals(block);
    mUi->referenceDataFromatComboBox->blockSignals(block);
    mUi->responseDataFormatComboBox->blockSignals(block);
    mUi->enableDelayCheckBox->blockSignals(block);
    mUi->delayTimeSpinBox->blockSignals(block);
}
