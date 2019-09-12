/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */
#include "SAKBase.hh"
#include "AutoResponseItemWidget.hh"
#include "ui_AutoResponseItemWidget.h"

#include <QDebug>
#include <QDateTime>

AutoResponseItemWidget::AutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::AutoResponseItemWidget)
    ,forbiddenAllAutoResponse (false)
    ,debugPage (debugPage)
{
    ui->setupUi(this);
    remarkLineEdit              = ui->remarkLineEdit;
    referenceLineEdit           = ui->referenceLineEdit;
    responseLineEdit            = ui->responseLineEdit;
    enableCheckBox              = ui->enableCheckBox;
    optionComboBox              = ui->optionComboBox;
    referenceDataFromatComboBox = ui->referenceDataFromatComboBox;
    responseDataFormatComboBox  = ui->responseDataFormatComboBox;

    optionComboBox->clear();
    optionComboBox->addItem(tr("接收数据与参考数据相同时自动回复"), QVariant::fromValue<int>(Equivalence));
    optionComboBox->addItem(tr("接收数据包含参考数据时自动回复"), QVariant::fromValue<int>(Contain));
    optionComboBox->addItem(tr("接收数据不包含参考数据时自动回复"), QVariant::fromValue<int>(Notcontain));

    SAKBase::instance()->initTextFormatComboBox(referenceDataFromatComboBox);
    SAKBase::instance()->initTextFormatComboBox(responseDataFormatComboBox);

    connect(debugPage, &SAKDebugPage::bytesRead, this, &AutoResponseItemWidget::dataRead);
    connect(this, &AutoResponseItemWidget::requestWrite, debugPage, &SAKDebugPage::write);
}

AutoResponseItemWidget::~AutoResponseItemWidget()
{
    delete ui;
}

void AutoResponseItemWidget::setAllAutoResponseDisable(bool disAbel)
{
    forbiddenAllAutoResponse = disAbel;
}

void AutoResponseItemWidget::setLineEditFormat(QLineEdit *lineEdit, int format)
{
    QRegExp regExpBin("([01][01][01][01][01][01][01][01][ ])*");
    QRegExp regExpOct("([0-7][0-7][ ])*");
    QRegExp regExpDec("([0-9][0-9][ ])*");
    QRegExp regExpHex("([0-9A-F][0-9A-F][ ])*");
    QRegExp regExpAscii("([a-zA-Z0-9`~!@#$%^&*()-_=+\\|;:'\",<.>/?])*");

    if (lineEdit){
        lineEdit->setValidator(nullptr);
        lineEdit->clear();
        switch (format) {
        case SAKBase::Bin:
            lineEdit->setValidator(new QRegExpValidator(regExpBin, this));
            break;
        case SAKBase::Oct:
            lineEdit->setValidator(new QRegExpValidator(regExpOct, this));
            break;
        case SAKBase::Dec:
            lineEdit->setValidator(new QRegExpValidator(regExpDec, this));
            break;
        case SAKBase::Hex:
            lineEdit->setValidator(new QRegExpValidator(regExpHex, this));
            break;
        case SAKBase::Ascii:
            lineEdit->setValidator(new QRegExpValidator(regExpAscii, this));
            break;
        case SAKBase::Utf8:
            lineEdit->setValidator(nullptr);
            break;
        case SAKBase::Local:
            lineEdit->setValidator(nullptr);
            break;
        default:
            break;
        }
    }
}

void AutoResponseItemWidget::dataRead(QByteArray data)
{
    if (forbiddenAllAutoResponse){
        return;
    }

    if (data.isEmpty()){
        return;
    }

    if (!enableCheckBox->isChecked()){
        return;
    }

    /*
     * 判断是否回复
     */
    QString referenceString = referenceLineEdit->text();
    int referenceFormat = referenceDataFromatComboBox->currentData().toInt();
    QByteArray referenceData = string2array(referenceString, referenceFormat);
    if (response(data, referenceData, optionComboBox->currentData().toInt())){
         QString responseString = responseLineEdit->text();
         int responseFromat = responseDataFormatComboBox->currentData().toInt();
         QByteArray responseData = string2array(responseString, responseFromat);

         if (!responseData.isEmpty()){
             debugPage->write(data);
         }
    }
}

QByteArray AutoResponseItemWidget::string2array(QString str, int format)
{
    auto stringList2Array = [](QStringList strList, int base) -> QByteArray{
        QByteArray array;
        for (auto var:strList){
            array.append(static_cast<char>(var.toInt(nullptr, base)));
        }
        return array;
    };

    QByteArray array;
    QStringList strList;
    int base;
    switch (format) {
    case SAKBase::Bin:
        base = 2;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKBase::Oct:
        base = 8;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKBase::Dec:
        base = 10;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKBase::Hex:
        base = 16;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKBase::Ascii:
        array = str.toLatin1();
        break;
    case SAKBase::Utf8:
        array = str.toUtf8();
        break;
    case SAKBase::Local:
        array = str.toLocal8Bit();
        break;
    default:
        array = str.toLatin1();
    }

    return array;
};

bool AutoResponseItemWidget::response(QByteArray receiveData, QByteArray referenceData, int option)
{
    if (option == AutoResponseItemWidget::Equivalence){
        if (QString(receiveData.toHex()).compare(QString(referenceData.toHex())) == 0){
            return true;
        }else{
            return false;
        }
    }

    if (option == AutoResponseItemWidget::Contain){
        if (QString(receiveData.toHex()).contains(QString(referenceData.toHex()))){
            return true;
        }else{
            return false;
        }
    }

    if (option == AutoResponseItemWidget::Notcontain){
        if (QString(receiveData.toHex()).contains(QString(referenceData.toHex()))){
            return false;
        }else{
            return true;
        }
    }

    return false;
};

void AutoResponseItemWidget::on_referenceDataFromatComboBox_currentTextChanged()
{
    setLineEditFormat(referenceLineEdit, referenceDataFromatComboBox->currentData().toInt());
}

void AutoResponseItemWidget::on_responseDataFormatComboBox_currentTextChanged()
{
    setLineEditFormat(responseLineEdit, responseDataFormatComboBox->currentData().toInt());
}
