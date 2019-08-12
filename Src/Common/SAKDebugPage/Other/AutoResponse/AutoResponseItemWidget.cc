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
#include "EDBaseApi.hh"
#include "AutoResponseItemWidget.hh"
#include "ui_AutoResponseItemWidget.h"

#include <QDebug>
#include <QDateTime>

AutoResponseItemWidget::AutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::AutoResponseItemWidget)
    ,forbiddenAllAutoResponse (false)
    ,_debugPage (debugPage)
{
    ui->setupUi(this);
    remarkLineEdit              = ui->remarkLineEdit;
    referenceLineEdit           = ui->referenceLineEdit;
    responseLineEdit            = ui->responseLineEdit;
    enableCheckBox              = ui->enableCheckBox;
    optionComboBox              = ui->optionComboBox;
    referenceDataFromatComboBox = ui->referenceDataFromatComboBox;
    responseDataFormatComboBox  = ui->responseDataFormatComboBox;

    QStringList optionStrings;
    optionStrings << tr("接收数据与参考数据相同时自动回复")
                  << tr("接收数据包含参考数据时自动回复")
                  << tr("接收数据不包含参考数据时自动回复");
    optionComboBox->clear();
    optionComboBox->addItems(optionStrings);

    EDBaseApi::instance()->initTextFormatComboBox(referenceDataFromatComboBox);
    EDBaseApi::instance()->initTextFormatComboBox(responseDataFormatComboBox);

    connect(_debugPage, &SAKDebugPage::dataReadOrwritten, this, &AutoResponseItemWidget::handleReceiceData);
    connect(this, &AutoResponseItemWidget::requestWrite, _debugPage, &SAKDebugPage::write);
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
    if (lineEdit){
        lineEdit->setValidator(nullptr);
        lineEdit->clear();
        switch (format) {
        case EDBaseApi::Bin:
            setLineEditFormatBin(lineEdit);
            break;
        case EDBaseApi::Oct:
            setLineEditFormatOct(lineEdit);
            break;
        case EDBaseApi::Dec:
            setLineEditFormatDec(lineEdit);
            break;
        case EDBaseApi::Hex:
            setLineEditFormatHex(lineEdit);
            break;
        case EDBaseApi::Ascii:
            setLineEditFormatAscii(lineEdit);
            break;
        case EDBaseApi::Utf8:
            setLineEditFormatUtf8(lineEdit);
            break;
        case EDBaseApi::Local:
            setLineEditFormatLocal(lineEdit);
            break;
        default:
            break;
        }
    }
}

void AutoResponseItemWidget::setLineEditFormatBin(QLineEdit *lineEdit)
{
    QRegExp regExp("([01][01][01][01][01][01][01][01][ ])*");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

void AutoResponseItemWidget::setLineEditFormatOct(QLineEdit *lineEdit)
{
    QRegExp regExp("([0-7][0-7][ ])*");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

void AutoResponseItemWidget::setLineEditFormatDec(QLineEdit *lineEdit)
{
    QRegExp regExp("([0-9][0-9][ ])*");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

void AutoResponseItemWidget::setLineEditFormatHex(QLineEdit *lineEdit)
{
    QRegExp regExp("([0-9A-F][0-9A-F][ ])*");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

void AutoResponseItemWidget::setLineEditFormatAscii(QLineEdit *lineEdit)
{
    QRegExp regExp("([a-zA-Z0-9`~!@#$%^&*()-_=+\\|;:'\",<.>/?])*");
    lineEdit->setValidator(new QRegExpValidator(regExp, this));
}

void AutoResponseItemWidget::setLineEditFormatUtf8(QLineEdit *lineEdit)
{
    lineEdit->setValidator(nullptr);
}

void AutoResponseItemWidget::setLineEditFormatLocal(QLineEdit *lineEdit)
{
    lineEdit->setValidator(nullptr);
}

void AutoResponseItemWidget::handleReceiceData(QByteArray data, SAKDebugPage::OutputParameters parameters)
{
    if (forbiddenAllAutoResponse){
        return;
    }

    if (data.isEmpty()){
        return;
    }

    if (!parameters.isReceivedData){
        return;
    }

    /*
     * 判断是否回复
     */
}

void AutoResponseItemWidget::on_referenceDataFromatComboBox_currentTextChanged()
{
    setLineEditFormat(referenceLineEdit, referenceDataFromatComboBox->currentData().toInt());
}

void AutoResponseItemWidget::on_responseDataFormatComboBox_currentTextChanged()
{
    setLineEditFormat(responseLineEdit, responseDataFormatComboBox->currentData().toInt());
}
