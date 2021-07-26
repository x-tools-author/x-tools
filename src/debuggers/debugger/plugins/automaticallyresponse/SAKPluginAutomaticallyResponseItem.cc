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
#include "SAKPluginAutomaticallyResponseItem.hh"

#include "ui_SAKPluginAutomaticallyResponseItem.h"

SAKPluginAutomaticallyResponseItem::SAKPluginAutomaticallyResponseItem(QWidget *parent)
    :QWidget(parent)
    ,mForbiddenAllAutoResponse(false)
    ,mUi(new Ui::SAKPluginAutomaticallyResponseItem)
{
    commonInitializing();
    blockUiSignals(false);
    mID = QDateTime::currentMSecsSinceEpoch();
    mDescriptionLineEdit->setText(QString::number(mID));
    initDelayWritingTimer();
}

SAKPluginAutomaticallyResponseItem::SAKPluginAutomaticallyResponseItem(quint64 id,
                                                   QString name,
                                                   QString referenceData,
                                                   QString responseData,
                                                   bool enabled,
                                                   quint32 referenceFormat,
                                                   quint32 responseFormat,
                                                   quint32 option,
                                                   bool delay,
                                                   int interval,
                                                   QWidget *parent)
    :QWidget(parent)
    ,mForbiddenAllAutoResponse(false)
    ,mID(id)
    ,mUi(new Ui::SAKPluginAutomaticallyResponseItem)
{
    commonInitializing();
    blockUiSignals(false);
    mDescriptionLineEdit->setText(name);
    mEnableCheckBox->setChecked(enabled);
    mReferenceDataFromatComboBox->setCurrentIndex(referenceFormat);
    mResponseDataFormatComboBox->setCurrentIndex(responseFormat);
    mReferenceLineEdit->setText(referenceData);
    mResponseLineEdit->setText(responseData);
    mOptionComboBox->setCurrentIndex(option);
    mDelayResponseCheckBox->setChecked(delay);
    mDelayResponseSpinBox->setValue(interval);
    initDelayWritingTimer();
}

SAKPluginAutomaticallyResponseItem::~SAKPluginAutomaticallyResponseItem()
{
    delete mUi;
}

void SAKPluginAutomaticallyResponseItem::setAllAutoResponseDisable(bool disable)
{
    mForbiddenAllAutoResponse = disable;
}

quint64 SAKPluginAutomaticallyResponseItem::itemID()
{
    return mID;
}

QString SAKPluginAutomaticallyResponseItem::itemDescription()
{
    return mDescriptionLineEdit->text();
}

QString SAKPluginAutomaticallyResponseItem::itemRefernceText()
{
    return mReferenceLineEdit->text();
}

QString SAKPluginAutomaticallyResponseItem::itemResponseText()
{
    return mResponseLineEdit->text();
}

bool SAKPluginAutomaticallyResponseItem::itemEnable()
{
    return mEnableCheckBox->isChecked();
}

quint32 SAKPluginAutomaticallyResponseItem::itemReferenceFormat()
{
    return mReferenceDataFromatComboBox->currentIndex();
}

quint32 SAKPluginAutomaticallyResponseItem::itemResponseFormat()
{
    return mResponseDataFormatComboBox->currentIndex();
}

quint32 SAKPluginAutomaticallyResponseItem::itemOption()
{
    return mOptionComboBox->currentIndex();
}

bool SAKPluginAutomaticallyResponseItem::delay()
{
    return mDelayResponseCheckBox->isChecked();
}

quint32 SAKPluginAutomaticallyResponseItem::interval()
{
    return mDescriptionLineEdit->text().toInt();
}

SAKPluginAutomaticallyResponseItem::SAKStructAutomaticallyResponseItemContext
SAKPluginAutomaticallyResponseItem::context()
{
    SAKStructAutomaticallyResponseItemContext ctx;
    ctx.id = mID;
    ctx.name = mUi->descriptionLineEdit->text();
    ctx.delay = mUi->delayResponseCheckBox->isChecked();
    ctx.enable = mUi->enableCheckBox->isChecked();
    ctx.option = mUi->optionComboBox->currentIndex();
    ctx.interval = mUi->delayResponseSpinBox->value();
    ctx.referenceData = mUi->referenceLineEdit->text();
    ctx.responseData = mUi->responseLineEdit->text();
    ctx.responseFormat = mUi->responseDataFormatComboBox->currentData().toInt();
    ctx.referenceFormat = mUi->referenceDataFromatComboBox->currentData().toInt();

    return ctx;
}

void SAKPluginAutomaticallyResponseItem::setLineEditFormat(QLineEdit *lineEdit, int format)
{
    if (lineEdit){
        lineEdit->clear();
        switch (format) {
        case SAKCommonDataStructure::InputFormatBin:
            SAKCommonInterface::setLineEditValidator(lineEdit, SAKCommonInterface::ValidatorBin);
            break;
        case SAKCommonDataStructure::InputFormatOct:
            SAKCommonInterface::setLineEditValidator(lineEdit, SAKCommonInterface::ValidatorOtc);
            break;
        case SAKCommonDataStructure::InputFormatDec:
            SAKCommonInterface::setLineEditValidator(lineEdit, SAKCommonInterface::ValidatorDec);
            break;
        case SAKCommonDataStructure::InputFormatHex:
            SAKCommonInterface::setLineEditValidator(lineEdit, SAKCommonInterface::ValidatorHex);
            break;
        case SAKCommonDataStructure::InputFormatAscii:
            SAKCommonInterface::setLineEditValidator(lineEdit, SAKCommonInterface::ValidatorAscii);
            break;
        default:
            SAKCommonInterface::setLineEditValidator(lineEdit, SAKCommonInterface::ValidatorNone);
            break;
        }
    }
}

void SAKPluginAutomaticallyResponseItem::bytesRead(QByteArray bytes)
{
    if (mForbiddenAllAutoResponse){
        return;
    }

    if (bytes.isEmpty()){
        return;
    }

    if (!mEnableCheckBox->isChecked()){
        return;
    }

    // Judge whether to reply or not
    QString referenceString;
    int referenceFormat = mReferenceDataFromatComboBox->currentData().toInt();
    if ((referenceFormat == SAKCommonDataStructure::InputFormatBin)
            || (referenceFormat == SAKCommonDataStructure::InputFormatOct)
            || (referenceFormat == SAKCommonDataStructure::InputFormatDec)
            || (referenceFormat == SAKCommonDataStructure::InputFormatHex)){
        referenceString = mReferenceLineEdit->text().trimmed();
    }else{
        referenceString = mReferenceLineEdit->text();
    }
    QByteArray referenceData = string2array(referenceString, referenceFormat);
    if (response(bytes, referenceData, mOptionComboBox->currentData().toInt())){
         QString responseString = mResponseLineEdit->text();
         int responseFromat = mResponseDataFormatComboBox->currentData().toInt();
         QByteArray responseData = string2array(responseString, responseFromat);

         if (!responseData.isEmpty()){
             // Response delayly
             if (mDelayResponseCheckBox->isChecked()){
                quint32 delayTime = mDelayResponseSpinBox->text().toUInt();
                if (delayTime < 40){
                    delayTime = 20;
                }

                // Response info
                DelayWritingInfo *info = new DelayWritingInfo;
                info->expectedTimestamp = QDateTime::currentMSecsSinceEpoch() + delayTime - 20;
                info->data = responseData;
                mWaitForWrittenInfoList.append(info);
             }else{
                 //mDebugPage->write(responseData);
             }
         }
    }
}

QByteArray SAKPluginAutomaticallyResponseItem::string2array(QString str, int format)
{
    return SAKCommonDataStructure::stringToByteArray(str, static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(format));
};

bool SAKPluginAutomaticallyResponseItem::response(QByteArray receiveData, QByteArray referenceData, int option)
{
    if (option == SAKCommonDataStructure::AutoResponseOptionEqual){
        return (QString(receiveData.toHex()).compare(QString(referenceData.toHex())) == 0);
    }

    if (option == SAKCommonDataStructure::AutoResponseOptionContain){
        return (QString(receiveData.toHex()).contains(QString(referenceData.toHex())));
    }

    if (option == SAKCommonDataStructure::AutoResponseOptionDoNotContain){
        return !(QString(receiveData.toHex()).contains(QString(referenceData.toHex())));
    }

    return false;
};

void SAKPluginAutomaticallyResponseItem::commonInitializing()
{
    mUi->setupUi(this);
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mReferenceLineEdit = mUi->referenceLineEdit;
    mResponseLineEdit = mUi->responseLineEdit;
    mEnableCheckBox = mUi->enableCheckBox;
    mOptionComboBox = mUi->optionComboBox;
    mReferenceDataFromatComboBox = mUi->referenceDataFromatComboBox;
    mResponseDataFormatComboBox  = mUi->responseDataFormatComboBox;
    mDelayResponseCheckBox = mUi->delayResponseCheckBox;
    mDelayResponseSpinBox = mUi->delayResponseSpinBox;

    blockUiSignals(true);

    mOptionComboBox->clear();
    mOptionComboBox->addItem(tr("Rx data is equal to reference data"), QVariant::fromValue<int>(SAKCommonDataStructure::AutoResponseOptionEqual));
    mOptionComboBox->addItem(tr("Rx data contains reference data"), QVariant::fromValue<int>(SAKCommonDataStructure::AutoResponseOptionContain));
    mOptionComboBox->addItem(tr("Rx data does not contains reference data"), QVariant::fromValue<int>(SAKCommonDataStructure::AutoResponseOptionDoNotContain));

    SAKCommonDataStructure::setComboBoxTextInputFormat(mReferenceDataFromatComboBox);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mResponseDataFormatComboBox);
}

void SAKPluginAutomaticallyResponseItem::initDelayWritingTimer()
{
    mTimestampChecker.setInterval(20);
    connect(&mTimestampChecker, &QTimer::timeout, this, &SAKPluginAutomaticallyResponseItem::delayToWritBytes);
    mTimestampChecker.start();
}

void SAKPluginAutomaticallyResponseItem::delayToWritBytes()
{
    mTimestampChecker.stop();
    QList<DelayWritingInfo> temp;
    QList<DelayWritingInfo*> need2removeList;
    for (int i = 0; i < mWaitForWrittenInfoList.length(); i++){
        DelayWritingInfo *infoPtr = mWaitForWrittenInfoList.at(i);
        if (quint64(QDateTime::currentMSecsSinceEpoch()) > infoPtr->expectedTimestamp){
            DelayWritingInfo info;
            info.data = infoPtr->data;
            info.expectedTimestamp = infoPtr->expectedTimestamp;
            temp.append(info);
            need2removeList.append(infoPtr);
        }
    }

    // Delte the data that has been written
    for (auto &var : need2removeList){
        mWaitForWrittenInfoList.removeOne(var);
    }
    mTimestampChecker.start();
}

void SAKPluginAutomaticallyResponseItem::blockUiSignals(bool block)
{
    mDescriptionLineEdit->blockSignals(block);
    mReferenceLineEdit->blockSignals(block);
    mResponseLineEdit->blockSignals(block);
    mEnableCheckBox->blockSignals(block);
    mOptionComboBox->blockSignals(block);
    mReferenceDataFromatComboBox->blockSignals(block);
    mResponseDataFormatComboBox->blockSignals(block);
    mDelayResponseCheckBox->blockSignals(block);
    mDelayResponseSpinBox->blockSignals(block);
}

void SAKPluginAutomaticallyResponseItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    emit descriptionChanged(text);
}

void SAKPluginAutomaticallyResponseItem::on_referenceLineEdit_textChanged(const QString &text)
{
    emit referenceTextChanged(text);
}

void SAKPluginAutomaticallyResponseItem::on_responseLineEdit_textChanged(const QString &text)
{
    emit responseTextChanged(text);
}

void SAKPluginAutomaticallyResponseItem::on_enableCheckBox_clicked()
{
    emit enableChanged(mEnableCheckBox->isChecked());
}

void SAKPluginAutomaticallyResponseItem::on_optionComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    int option = mOptionComboBox->currentData().toInt();
    emit optionChanged(option);
}

void SAKPluginAutomaticallyResponseItem::on_referenceDataFromatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    setLineEditFormat(mReferenceLineEdit, mReferenceDataFromatComboBox->currentData().toInt());

    int format = mReferenceDataFromatComboBox->currentData().toInt();
    emit referenceFormatChanged(format);
}

void SAKPluginAutomaticallyResponseItem::on_responseDataFormatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    setLineEditFormat(mResponseLineEdit, mResponseDataFormatComboBox->currentData().toInt());

    int format = mResponseDataFormatComboBox->currentData().toInt();
    emit responseFromatChanged(format);
}

void SAKPluginAutomaticallyResponseItem::on_delayResponseCheckBox_clicked()
{
    emit delayChanged(mDelayResponseCheckBox->isChecked());
}

void SAKPluginAutomaticallyResponseItem::on_delayResponseLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    emit intervalChanged(interval);
}
