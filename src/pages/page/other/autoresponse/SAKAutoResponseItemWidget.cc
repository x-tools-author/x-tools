/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDateTime>

#include "SAKGlobal.hh"
#include "SAKDataStruct.hh"
#include "SAKAutoResponseItemWidget.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKAutoResponseItemWidget.h"

SAKAutoResponseItemWidget::SAKAutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,forbiddenAllAutoResponse(false)
    ,debugPage(debugPage)
    ,ui(new Ui::SAKAutoResponseItemWidget)
{
    initUi();
    id = QDateTime::currentMSecsSinceEpoch();
    remarkLineEdit->setText(QString::number(id));
}

SAKAutoResponseItemWidget::SAKAutoResponseItemWidget(SAKDebugPage *debugPage,
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
    ,forbiddenAllAutoResponse(false)
    ,debugPage(debugPage)
    ,id(id)
    ,ui(new Ui::SAKAutoResponseItemWidget)
{
    initUi();
    remarkLineEdit->setText(name);
    referenceLineEdit->setText(referenceData);
    responseLineEdit->setText(responseData);
    enableCheckBox->setChecked(enabled);
    referenceDataFromatComboBox->setCurrentIndex(referenceFormat);
    responseDataFormatComboBox->setCurrentIndex(responseFormat);
    optionComboBox->setCurrentIndex(option);
}

SAKAutoResponseItemWidget::~SAKAutoResponseItemWidget()
{
    delete ui;
}

void SAKAutoResponseItemWidget::setAllAutoResponseDisable(bool disable)
{
    forbiddenAllAutoResponse = disable;
}

quint64 SAKAutoResponseItemWidget::parameterID()
{
    return id;
}

QString SAKAutoResponseItemWidget::parameterName()
{
    return remarkLineEdit->text();
}

QString SAKAutoResponseItemWidget::parameterRefernceData()
{
    return referenceLineEdit->text();
}

QString SAKAutoResponseItemWidget::parameterResponseData()
{
    return responseLineEdit->text();
}

bool SAKAutoResponseItemWidget::parameterEnable()
{
    return enableCheckBox->isChecked();
}

quint32 SAKAutoResponseItemWidget::parameterReferenceFormat()
{
    return referenceDataFromatComboBox->currentIndex();
}

quint32 SAKAutoResponseItemWidget::parameterResponseFormat()
{
    return responseDataFormatComboBox->currentIndex();
}

quint32 SAKAutoResponseItemWidget::parameterOption()
{
    return optionComboBox->currentIndex();
}

void SAKAutoResponseItemWidget::setLineEditFormat(QLineEdit *lineEdit, int format)
{
    QRegExp regExpBin("([01][01][01][01][01][01][01][01][ ])*");
    QRegExp regExpOct("([0-7][0-7][ ])*");
    QRegExp regExpDec("([0-9][0-9][ ])*");
    QRegExp regExpHex("([0-9A-F][0-9A-F][ ])*");
    QRegExp regExpAscii("([a-zA-Z0-9`~!@#$%^&*()-_=+\\|;:'\",<.>/?])*");

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

void SAKAutoResponseItemWidget::bytesRead(QByteArray bytes)
{
    if (forbiddenAllAutoResponse){
        return;
    }

    if (bytes.isEmpty()){
        return;
    }

    if (!enableCheckBox->isChecked()){
        return;
    }

    /// @brief 判断是否回复
    QString referenceString = referenceLineEdit->text();
    int referenceFormat = referenceDataFromatComboBox->currentData().toInt();
    QByteArray referenceData = string2array(referenceString, referenceFormat);
    if (response(bytes, referenceData, optionComboBox->currentData().toInt())){
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

bool SAKAutoResponseItemWidget::response(QByteArray receiveData, QByteArray referenceData, int option)
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

void SAKAutoResponseItemWidget::initUi()
{
    ui->setupUi(this);
    remarkLineEdit = ui->remarkLineEdit;
    referenceLineEdit = ui->referenceLineEdit;
    responseLineEdit = ui->responseLineEdit;
    enableCheckBox = ui->enableCheckBox;
    optionComboBox = ui->optionComboBox;
    updatePushButton = ui->updatePushButton;
    referenceDataFromatComboBox = ui->referenceDataFromatComboBox;
    responseDataFormatComboBox  = ui->responseDataFormatComboBox;

    optionComboBox->clear();
    optionComboBox->addItem(tr("接收数据等于参考数据时自动回复"), QVariant::fromValue<int>(SAKDataStruct::AutoResponseOptionEqual));
    optionComboBox->addItem(tr("接收数据包含参考数据时自动回复"), QVariant::fromValue<int>(SAKDataStruct::AutoResponseOptionContain));
    optionComboBox->addItem(tr("接收数据不包含参考数据时自动回复"), QVariant::fromValue<int>(SAKDataStruct::AutoResponseOptionDoNotContain));

    SAKGlobal::initInputTextFormatComboBox(referenceDataFromatComboBox);
    SAKGlobal::initInputTextFormatComboBox(responseDataFormatComboBox);

    connect(debugPage, &SAKDebugPage::bytesRead, this, &SAKAutoResponseItemWidget::bytesRead);
}

void SAKAutoResponseItemWidget::on_referenceDataFromatComboBox_currentTextChanged()
{
    setLineEditFormat(referenceLineEdit, referenceDataFromatComboBox->currentData().toInt());
}

void SAKAutoResponseItemWidget::on_responseDataFormatComboBox_currentTextChanged()
{
    setLineEditFormat(responseLineEdit, responseDataFormatComboBox->currentData().toInt());
}

void SAKAutoResponseItemWidget::on_updatePushButton_clicked()
{
    QString tableName = SAKDataStruct::autoResponseTableName(debugPage->pageType());
    SAKDataStruct::SAKStructAutoResponseItem item;
    item.id = parameterID();
    item.name = parameterName();
    item.enable = parameterEnable();
    item.responseData = parameterResponseData();
    item.referenceData = parameterRefernceData();
    item.responseFormat = parameterResponseFormat();
    item.referenceFormat = parameterReferenceFormat();
    item.option = parameterOption();
    SAKDebugPageDatabaseInterface::instance()->updateAutoResponseItem(tableName, item);
}
