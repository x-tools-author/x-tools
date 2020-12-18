/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include "SAKToolStringAssistant.hh"
#include "SAKCommonDataStructure.hh"
#include "ui_SAKToolStringAssistant.h"

SAKToolStringAssistant::SAKToolStringAssistant(QWidget *parent)
    :QDialog(parent)
    ,ui(new Ui::SAKToolStringAssistant)
{
    ui->setupUi(this);
    SAKCommonDataStructure::setComboBoxTextInputFormat(ui->inputFormatComboBox);
    SAKCommonDataStructure::setComboBoxTextOutputFormat(ui->outputFormatComboBox);
}

SAKToolStringAssistant::~SAKToolStringAssistant()
{
    delete ui;
}

void SAKToolStringAssistant::on_textEdit_textChanged()
{
    if (!ui->textEdit->blockSignals(true)){
        QString inputString = ui->textEdit->toPlainText();
        auto inputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(ui->inputFormatComboBox->currentData().toInt());
        QString cookedString = SAKCommonDataStructure::formattingString(inputString, inputFormat);
        ui->textEdit->setText(cookedString);
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->blockSignals(false);
        on_createPushButton_clicked();
    }else{
        Q_ASSERT_X(false, __FUNCTION__, "Oh, No!");
    }
}

void SAKToolStringAssistant::on_inputFormatComboBox_currentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->textEdit->clear();
    on_createPushButton_clicked();
}

void SAKToolStringAssistant::on_createPushButton_clicked()
{
    QString inputString = ui->textEdit->toPlainText();
    auto inputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextInputFormat>(ui->inputFormatComboBox->currentData().toInt());
    QByteArray inputArray = SAKCommonDataStructure::stringToByteArray(inputString, inputFormat);
    auto outputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextOutputFormat>(ui->outputFormatComboBox->currentData().toInt());
    auto outputString = SAKCommonDataStructure::byteArrayToString(inputArray, outputFormat);
    ui->textBrowser->setText(outputString);
}

void SAKToolStringAssistant::on_outputFormatComboBox_currentTextChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    on_createPushButton_clicked();
}
