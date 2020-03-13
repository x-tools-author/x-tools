/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 */
#include <QDateTime>

#include "SAKGlobal.hh"
#include "SAKAutoResponseItemWidget.hh"

#include "ui_SAKAutoResponseItemWidget.h"

SAKAutoResponseItemWidget::SAKAutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::SAKAutoResponseItemWidget)
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

    SAKGlobal::initInputTextFormatComboBox(referenceDataFromatComboBox);
    SAKGlobal::initInputTextFormatComboBox(responseDataFormatComboBox);

    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKAutoResponseItemWidget::dataRead);
    connect(this, &SAKAutoResponseItemWidget::requestWrite, debugPage, &SAKDebugPage::write);
}

SAKAutoResponseItemWidget::~SAKAutoResponseItemWidget()
{
    delete ui;
}

void SAKAutoResponseItemWidget::setAllAutoResponseDisable(bool disAbel)
{
    forbiddenAllAutoResponse = disAbel;
}

void SAKAutoResponseItemWidget::setLineEditFormat(QLineEdit *lineEdit, int format)
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
        case SAKGlobal::Ibin:
            lineEdit->setValidator(new QRegExpValidator(regExpBin, this));
            break;
        case SAKGlobal::Ioct:
            lineEdit->setValidator(new QRegExpValidator(regExpOct, this));
            break;
        case SAKGlobal::Idec:
            lineEdit->setValidator(new QRegExpValidator(regExpDec, this));
            break;
        case SAKGlobal::Ihex:
            lineEdit->setValidator(new QRegExpValidator(regExpHex, this));
            break;
        case SAKGlobal::Iascii:
            lineEdit->setValidator(new QRegExpValidator(regExpAscii, this));
            break;
        case SAKGlobal::Ilocal:
            lineEdit->setValidator(nullptr);
            break;
        default:
            break;
        }
    }
}

void SAKAutoResponseItemWidget::dataRead(QByteArray data)
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
             debugPage->write(responseData);
         }
    }
}

QByteArray SAKAutoResponseItemWidget::string2array(QString str, int format)
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
    case SAKGlobal::Ibin:
        base = 2;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKGlobal::Ioct:
        base = 8;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKGlobal::Idec:
        base = 10;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKGlobal::Ihex:
        base = 16;
        strList = str.split(' ');
        array = stringList2Array(strList, base);
        break;
    case SAKGlobal::Iascii:
        array = str.toLatin1();
        break;
    case SAKGlobal::Ilocal:
        array = str.toLocal8Bit();
        break;
    default:
        array = str.toLatin1();
    }

    return array;
};

bool SAKAutoResponseItemWidget::response(QByteArray receiveData, QByteArray referenceData, int option)
{
    if (option == SAKAutoResponseItemWidget::Equivalence){
        if (QString(receiveData.toHex()).compare(QString(referenceData.toHex())) == 0){
            return true;
        }else{
            return false;
        }
    }

    if (option == SAKAutoResponseItemWidget::Contain){
        if (QString(receiveData.toHex()).contains(QString(referenceData.toHex()))){
            return true;
        }else{
            return false;
        }
    }

    if (option == SAKAutoResponseItemWidget::Notcontain){
        if (QString(receiveData.toHex()).contains(QString(referenceData.toHex()))){
            return false;
        }else{
            return true;
        }
    }

    return false;
};

void SAKAutoResponseItemWidget::on_referenceDataFromatComboBox_currentTextChanged()
{
    setLineEditFormat(referenceLineEdit, referenceDataFromatComboBox->currentData().toInt());
}

void SAKAutoResponseItemWidget::on_responseDataFormatComboBox_currentTextChanged()
{
    setLineEditFormat(responseLineEdit, responseDataFormatComboBox->currentData().toInt());
}
