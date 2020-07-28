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

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKOtherAutoResponseItem.hh"

#include "ui_SAKOtherAutoResponseItem.h"

SAKOtherAutoResponseItem::SAKOtherAutoResponseItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mForbiddenAllAutoResponse(false)
    ,mDebugPage(debugPage)
    ,mUi(new Ui::SAKOtherAutoResponseItem)
{
    initUi();
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
                                                   QWidget *parent)
    :QWidget(parent)
    ,mForbiddenAllAutoResponse(false)
    ,mDebugPage(debugPage)
    ,mID(id)
    ,mUi(new Ui::SAKOtherAutoResponseItem)
{
    initUi();
    mDescriptionLineEdit->setText(name);
    mReferenceLineEdit->setText(referenceData);
    mResponseLineEdit->setText(responseData);
    enableCheckBox->setChecked(enabled);
    mReferenceDataFromatComboBox->setCurrentIndex(referenceFormat);
    mResponseDataFormatComboBox->setCurrentIndex(responseFormat);
    mOptionComboBox->setCurrentIndex(option);
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
    return enableCheckBox->isChecked();
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

void SAKOtherAutoResponseItem::setLineEditFormat(QLineEdit *lineEdit, int format)
{
    QRegExp regExpBin("([01][01][01][01][01][01][01][01][ ])*");
    QRegExp regExpOct("([0-7][0-7][ ])*");
    QRegExp regExpDec("([0-9][0-9][ ])*");
    QRegExp regExpHex("([0-9A-F][0-9A-F][ ])*");
    QRegExp regExpAscii("([a-zA-Z0-9`~!@#$%^&*()-_=+\\|;:'\",<.>/? ])*");

    if (lineEdit){
        lineEdit->setValidator(Q_NULLPTR);
        lineEdit->clear();
        switch (format) {
        case SAKDataStruct::InputFormatBin:
            lineEdit->setValidator(new QRegExpValidator(regExpBin, this));
            break;
        case SAKDataStruct::InputFormatOct:
            lineEdit->setValidator(new QRegExpValidator(regExpOct, this));
            break;
        case SAKDataStruct::InputFormatDec:
            lineEdit->setValidator(new QRegExpValidator(regExpDec, this));
            break;
        case SAKDataStruct::InputFormatHex:
            lineEdit->setValidator(new QRegExpValidator(regExpHex, this));
            break;
        case SAKDataStruct::InputFormatAscii:
            lineEdit->setValidator(new QRegExpValidator(regExpAscii, this));
            break;
        case SAKDataStruct::InputFormatLocal:
            lineEdit->setValidator(Q_NULLPTR);
            break;
        default:
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

    if (!enableCheckBox->isChecked()){
        return;
    }

    /// @brief 判断是否回复
    QString referenceString = mReferenceLineEdit->text();
    int referenceFormat = mReferenceDataFromatComboBox->currentData().toInt();
    QByteArray referenceData = string2array(referenceString, referenceFormat);
    if (response(bytes, referenceData, mOptionComboBox->currentData().toInt())){
         QString responseString = mResponseLineEdit->text();
         int responseFromat = mResponseDataFormatComboBox->currentData().toInt();
         QByteArray responseData = string2array(responseString, responseFromat);

         if (!responseData.isEmpty()){
             /// @brief 延时回复
             if (mDelayResponseCheckBox->isChecked()){
                quint32 delayTime = mDelayResponseLineEdit->text().toUInt();
                if (delayTime < 40){
                    delayTime = 20;
                }

                /// @brief 延时回复
                DelayWritingInfo *info = new DelayWritingInfo;
                /// @brief 减20是因为延时回复使用20毫秒的定时器
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
    auto stringList2Array = [](QStringList strList, int base) -> QByteArray{
        QByteArray array;
        for (auto var:strList){
            array.append(static_cast<char>(var.toInt(Q_NULLPTR, base)));
        }
        return array;
    };

    QByteArray array;
    QStringList strList;
    int base;
    switch (format) {
    case SAKDataStruct::InputFormatBin:
        base = 2;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKDataStruct::InputFormatOct:
        base = 8;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKDataStruct::InputFormatDec:
        base = 10;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKDataStruct::InputFormatHex:
        base = 16;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKDataStruct::InputFormatAscii:
        array = str.toLatin1();
        break;
    case SAKDataStruct::InputFormatLocal:
        array = str.toLocal8Bit();
        break;
    default:
        array = str.toLatin1();
    }

    return array;
};

bool SAKOtherAutoResponseItem::response(QByteArray receiveData, QByteArray referenceData, int option)
{
    if (option == SAKDataStruct::AutoResponseOptionEqual){
        return (QString(receiveData.toHex()).compare(QString(referenceData.toHex())) == 0);
    }

    if (option == SAKDataStruct::AutoResponseOptionContain){
        return (QString(receiveData.toHex()).contains(QString(referenceData.toHex())));
    }

    if (option == SAKDataStruct::AutoResponseOptionDoNotContain){
        return !(QString(receiveData.toHex()).contains(QString(referenceData.toHex())));
    }

    return false;
};

void SAKOtherAutoResponseItem::initUi()
{
    mUi->setupUi(this);
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mReferenceLineEdit = mUi->referenceLineEdit;
    mResponseLineEdit = mUi->responseLineEdit;
    enableCheckBox = mUi->enableCheckBox;
    mOptionComboBox = mUi->optionComboBox;
    mReferenceDataFromatComboBox = mUi->referenceDataFromatComboBox;
    mResponseDataFormatComboBox  = mUi->responseDataFormatComboBox;
    mDelayResponseCheckBox = mUi->delayResponseCheckBox;
    mDelayResponseLineEdit = mUi->delayResponseLineEdit;

    mOptionComboBox->clear();
    mOptionComboBox->addItem(tr("接收数据等于参考数据时自动回复"), QVariant::fromValue<int>(SAKDataStruct::AutoResponseOptionEqual));
    mOptionComboBox->addItem(tr("接收数据包含参考数据时自动回复"), QVariant::fromValue<int>(SAKDataStruct::AutoResponseOptionContain));
    mOptionComboBox->addItem(tr("接收数据不包含参考数据时自动回复"), QVariant::fromValue<int>(SAKDataStruct::AutoResponseOptionDoNotContain));

    SAKGlobal::initInputTextFormatComboBox(mReferenceDataFromatComboBox);
    SAKGlobal::initInputTextFormatComboBox(mResponseDataFormatComboBox);

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

    /// @brief 发送数据
    for (auto var : temp){
        mDebugPage->write(var.data);
    }

    /// @brief 删除已发送的数据
    for (auto var : need2removeList){
        mWaitForWrittenInfoList.removeOne(var);
    }
    mTimestampChecker.start();
}

void SAKOtherAutoResponseItem::on_descriptionLineEdit_textChanged(const QString &text)
{

}

void SAKOtherAutoResponseItem::on_referenceLineEdit_textChanged(const QString &text)
{

}

void SAKOtherAutoResponseItem::on_responseLineEdit_textChanged(const QString &text)
{

}

void SAKOtherAutoResponseItem::on_enableCheckBox_clicked()
{

}

void SAKOtherAutoResponseItem::on_optionComboBox_currentIndexChanged(const QString &text)
{

}

void SAKOtherAutoResponseItem::on_referenceDataFromatComboBox_currentIndexChanged(const QString &text)
{
    setLineEditFormat(mReferenceLineEdit, mReferenceDataFromatComboBox->currentData().toInt());
}

void SAKOtherAutoResponseItem::on_responseDataFormatComboBox_currentIndexChanged(const QString &text)
{
    setLineEditFormat(mResponseLineEdit, mResponseDataFormatComboBox->currentData().toInt());
}

void SAKOtherAutoResponseItem::on_delayResponseCheckBox_clicked()
{

}

void SAKOtherAutoResponseItem::on_delayResponseLineEdit_textChanged(const QString &text)
{

}

void SAKOtherAutoResponseItem::on_referenceDataFromatComboBox_currentTextChanged()
{
    setLineEditFormat(mReferenceLineEdit, mReferenceDataFromatComboBox->currentData().toInt());
}

void SAKOtherAutoResponseItem::on_responseDataFormatComboBox_currentTextChanged()
{
    setLineEditFormat(mResponseLineEdit, mResponseDataFormatComboBox->currentData().toInt());
}
