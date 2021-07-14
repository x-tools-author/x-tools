/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QDateTime>
#include <QRegularExpression>
#include <QRegularExpressionValidator>

#include "SAKDebugPage.hh"
#include "SAKCommonInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKOtherAutoResponseItem.hh"

#include "ui_SAKOtherAutoResponseItem.h"

SAKOtherAutoResponseItem::SAKOtherAutoResponseItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mForbiddenAllAutoResponse(false)
    ,mDebugPage(debugPage)
    ,mUi(new Ui::SAKOtherAutoResponseItem)
{
    commonInitializing();
    blockUiSignals(false);
    mID = QDateTime::currentMSecsSinceEpoch();
    mDescriptionLineEdit->setText(QString::number(mID));
    initDelayWritingTimer();
}

SAKOtherAutoResponseItem::SAKOtherAutoResponseItem(SAKDebugPage *debugPage,
                                                   quint64 id,
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
    ,mDebugPage(debugPage)
    ,mID(id)
    ,mUi(new Ui::SAKOtherAutoResponseItem)
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
    mDelayResponseLineEdit->setText(QString::number(interval));
    initDelayWritingTimer();
}

SAKOtherAutoResponseItem::~SAKOtherAutoResponseItem()
{
    delete mUi;
}

void SAKOtherAutoResponseItem::setAllAutoResponseDisable(bool disable)
{
    mForbiddenAllAutoResponse = disable;
}

quint64 SAKOtherAutoResponseItem::itemID()
{
    return mID;
}

QString SAKOtherAutoResponseItem::itemDescription()
{
    return mDescriptionLineEdit->text();
}

QString SAKOtherAutoResponseItem::itemRefernceText()
{
    return mReferenceLineEdit->text();
}

QString SAKOtherAutoResponseItem::itemResponseText()
{
    return mResponseLineEdit->text();
}

bool SAKOtherAutoResponseItem::itemEnable()
{
    return mEnableCheckBox->isChecked();
}

quint32 SAKOtherAutoResponseItem::itemReferenceFormat()
{
    return mReferenceDataFromatComboBox->currentIndex();
}

quint32 SAKOtherAutoResponseItem::itemResponseFormat()
{
    return mResponseDataFormatComboBox->currentIndex();
}

quint32 SAKOtherAutoResponseItem::itemOption()
{
    return mOptionComboBox->currentIndex();
}

bool SAKOtherAutoResponseItem::delay()
{
    return mDelayResponseCheckBox->isChecked();
}

quint32 SAKOtherAutoResponseItem::interval()
{
    return mDescriptionLineEdit->text().toInt();
}

void SAKOtherAutoResponseItem::setLineEditFormat(QLineEdit *lineEdit, int format)
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

void SAKOtherAutoResponseItem::bytesRead(QByteArray bytes)
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
                quint32 delayTime = mDelayResponseLineEdit->text().toUInt();
                if (delayTime < 40){
                    delayTime = 20;
                }

                // Response info
                DelayWritingInfo *info = new DelayWritingInfo;
                info->expectedTimestamp = QDateTime::currentMSecsSinceEpoch() + delayTime - 20;
                info->data = responseData;
                mWaitForWrittenInfoList.append(info);
             }else{
                 mDebugPage->write(responseData);
             }
         }
    }
}

QByteArray SAKOtherAutoResponseItem::string2array(QString str, int format)
{
    return SAKCommonDataStructure::stringToByteArray(str, static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(format));
};

bool SAKOtherAutoResponseItem::response(QByteArray receiveData, QByteArray referenceData, int option)
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

void SAKOtherAutoResponseItem::commonInitializing()
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
    mDelayResponseLineEdit = mUi->delayResponseLineEdit;

    blockUiSignals(true);

    mOptionComboBox->clear();
    mOptionComboBox->addItem(tr("Rx data is equal to reference data"), QVariant::fromValue<int>(SAKCommonDataStructure::AutoResponseOptionEqual));
    mOptionComboBox->addItem(tr("Rx data contains reference data"), QVariant::fromValue<int>(SAKCommonDataStructure::AutoResponseOptionContain));
    mOptionComboBox->addItem(tr("Rx data does not contains reference data"), QVariant::fromValue<int>(SAKCommonDataStructure::AutoResponseOptionDoNotContain));

    SAKCommonDataStructure::setComboBoxTextInputFormat(mReferenceDataFromatComboBox);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mResponseDataFormatComboBox);

    connect(mDebugPage, &SAKDebugPage::bytesRead, this, &SAKOtherAutoResponseItem::bytesRead);
}

void SAKOtherAutoResponseItem::initDelayWritingTimer()
{
    mTimestampChecker.setInterval(20);
    connect(&mTimestampChecker, &QTimer::timeout, this, &SAKOtherAutoResponseItem::delayToWritBytes);
    mTimestampChecker.start();
}

void SAKOtherAutoResponseItem::delayToWritBytes()
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

    // Write data
    for (auto &var : temp){
        mDebugPage->write(var.data);
    }

    // Delte the data that has been written
    for (auto &var : need2removeList){
        mWaitForWrittenInfoList.removeOne(var);
    }
    mTimestampChecker.start();
}

void SAKOtherAutoResponseItem::blockUiSignals(bool block)
{
    mDescriptionLineEdit->blockSignals(block);
    mReferenceLineEdit->blockSignals(block);
    mResponseLineEdit->blockSignals(block);
    mEnableCheckBox->blockSignals(block);
    mOptionComboBox->blockSignals(block);
    mReferenceDataFromatComboBox->blockSignals(block);
    mResponseDataFormatComboBox->blockSignals(block);
    mDelayResponseCheckBox->blockSignals(block);
    mDelayResponseLineEdit->blockSignals(block);
}

void SAKOtherAutoResponseItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    emit descriptionChanged(text);
}

void SAKOtherAutoResponseItem::on_referenceLineEdit_textChanged(const QString &text)
{
    emit referenceTextChanged(text);
}

void SAKOtherAutoResponseItem::on_responseLineEdit_textChanged(const QString &text)
{
    emit responseTextChanged(text);
}

void SAKOtherAutoResponseItem::on_enableCheckBox_clicked()
{
    emit enableChanged(mEnableCheckBox->isChecked());
}

void SAKOtherAutoResponseItem::on_optionComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    int option = mOptionComboBox->currentData().toInt();
    emit optionChanged(option);
}

void SAKOtherAutoResponseItem::on_referenceDataFromatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    setLineEditFormat(mReferenceLineEdit, mReferenceDataFromatComboBox->currentData().toInt());

    int format = mReferenceDataFromatComboBox->currentData().toInt();
    emit referenceFormatChanged(format);
}

void SAKOtherAutoResponseItem::on_responseDataFormatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    setLineEditFormat(mResponseLineEdit, mResponseDataFormatComboBox->currentData().toInt());

    int format = mResponseDataFormatComboBox->currentData().toInt();
    emit responseFromatChanged(format);
}

void SAKOtherAutoResponseItem::on_delayResponseCheckBox_clicked()
{
    emit delayChanged(mDelayResponseCheckBox->isChecked());
}

void SAKOtherAutoResponseItem::on_delayResponseLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    emit intervalChanged(interval);
}
