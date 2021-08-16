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
    setupItem();
}

SAKDebuggerPluginAutoResponseItem::SAKDebuggerPluginAutoResponseItem(
        SAKStructItemContext ctx,
        QWidget *parent)
    :SAKBaseListWidgetItemWidget(ctx.id, parent)
    ,mUi(new Ui::SAKDebuggerPluginAutoResponseItem)
{
    mUi->setupUi(this);
    setupItem();

    mUi->descriptionLineEdit->setText(ctx.description);
    mUi->referenceLineEdit->setText(ctx.referenceData);
    mUi->responseLineEdit->setText(ctx.responseData);
    mUi->enableCheckBox->setChecked(ctx.enable);
    mUi->referenceDataFromatComboBox->setCurrentIndex(ctx.referenceFormat);
    mUi->responseDataFormatComboBox->setCurrentIndex(ctx.responseFormat);
    mUi->optionComboBox->setCurrentIndex(ctx.option);
    mUi->delayResponseCheckBox->setChecked(ctx.enableDelay);
    mUi->delayResponseSpinBox->setValue(ctx.delayTime);
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
    ctx.enableDelay = mUi->delayResponseCheckBox->isChecked();
    ctx.enable = mUi->enableCheckBox->isChecked();
    ctx.option = mUi->optionComboBox->currentIndex();
    ctx.delayTime = mUi->delayResponseSpinBox->value();
    ctx.referenceData = mUi->referenceLineEdit->text();
    ctx.responseData = mUi->responseLineEdit->text();
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
        referenceString = mUi->referenceLineEdit->text().trimmed();
    } else {
        referenceString = mUi->referenceLineEdit->text();
    }


    auto cookedReferenceFormat =
            static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(referenceFormat);
    QByteArray referenceData =
            SAKCommonDataStructure::stringToByteArray(referenceString,
                                                      cookedReferenceFormat);
    if (needToResponse(bytes, referenceData, mUi->optionComboBox->currentData().toInt())) {
         QString responseString = mUi->responseLineEdit->text();
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
             if (mUi->delayResponseCheckBox->isChecked()) {
                 int delay = mUi->delayResponseSpinBox->value();
                 QTimer::singleShot(delay, this, [=](){
                     emit invokeWriteBytes(responseData);
                 });
             } else {
                 emit invokeWriteBytes(responseData);
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


    auto blockUiComponentsSignals = [&](bool block){
        mUi->descriptionLineEdit->blockSignals(block);
        mUi->referenceLineEdit->blockSignals(block);
        mUi->responseLineEdit->blockSignals(block);
        mUi->optionComboBox->blockSignals(block);
        mUi->referenceDataFromatComboBox->blockSignals(block);
        mUi->responseDataFormatComboBox->blockSignals(block);
        mUi->delayResponseCheckBox->blockSignals(block);
        mUi->delayResponseSpinBox->blockSignals(block);
    };

    blockUiComponentsSignals(true);
    connect(mUi->descriptionLineEdit, &QLineEdit::textChanged,
            this, [&](const QString description){
        emit descriptionChanged(id(), description);
    });

    connect(mUi->referenceLineEdit, &QLineEdit::textChanged,
            this, [&](const QString description){
        emit referenceTextChanged(id(), description);
    });

    connect(mUi->responseLineEdit, &QLineEdit::textChanged,
            this, [&](const QString description){
        emit responseTextChanged(id(), description);
    });


    connect(mUi->optionComboBox, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int option){
        emit optionChanged(id(), option);
    });


    connect(mUi->referenceDataFromatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int format){
        emit referenceFormatChanged(id(), format);
    });

    connect(mUi->responseDataFormatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, [&](int format){
        emit responseFromatChanged(id(), format);
    });


    connect(mUi->delayResponseCheckBox, &QCheckBox::clicked,
            this, [&](){
        emit enableDelayChanged(id(), mUi->delayResponseCheckBox->isChecked());
    });


    connect(mUi->delayResponseSpinBox, QOverload<int>::of(&QSpinBox::valueChanged),
            this, [&](int interval){
        emit delayTimeChanged(id(), interval);
    });
    blockUiComponentsSignals(false);
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
};
