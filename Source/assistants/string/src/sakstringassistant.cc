/***************************************************************************************************
 * Copyright 2020-2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakstringassistant.h"
#include "ui_sakstringassistant.h"

#include "sakdatastructure.h"

SAKStringAssistant::SAKStringAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SAKStringAssistant)
{
    ui->setupUi(this);
    SAKDataStructure::setComboBoxTextInputFormat(ui->inputFormatComboBox);
    SAKDataStructure::setComboBoxTextOutputFormat(ui->outputFormatComboBox);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &SAKStringAssistant::onTextEditTextChanged);
    connect(ui->inputFormatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SAKStringAssistant::onInputFormatComboBoxCurrentIndexChanged);
    connect(ui->createPushButton,
            &QPushButton::clicked,
            this,
            &SAKStringAssistant::onCreatePushButtonClicked);
    connect(ui->outputFormatComboBox,
            &QComboBox::currentTextChanged,
            this,
            &SAKStringAssistant::onOutputFormatComboBoxCurrentTextChanged);
}

SAKStringAssistant::~SAKStringAssistant()
{
    delete ui;
}

void SAKStringAssistant::onTextEditTextChanged()
{
    if (!ui->textEdit->blockSignals(true)) {
        QString inputString = ui->textEdit->toPlainText();
        auto inputFormat = static_cast<SAKDataStructure::SAKEnumTextFormatInput>(
            ui->inputFormatComboBox->currentData().toInt());
        QString cookedString = SAKDataStructure::formattingString(inputString, inputFormat);
        ui->textEdit->setText(cookedString);
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->blockSignals(false);
        onCreatePushButtonClicked();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Oh, No!");
    }
}

void SAKStringAssistant::onInputFormatComboBoxCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->textEdit->clear();
    onCreatePushButtonClicked();
}

void SAKStringAssistant::onCreatePushButtonClicked()
{
    QString inputString = ui->textEdit->toPlainText();
    auto inputFormat = static_cast<SAKDataStructure::SAKEnumTextFormatInput>(
        ui->inputFormatComboBox->currentData().toInt());
    QByteArray inputArray = SAKDataStructure::stringToByteArray(inputString, inputFormat);
    auto outputFormat = static_cast<SAKDataStructure::SAKEnumTextFormatOutput>(
        ui->outputFormatComboBox->currentData().toInt());
    auto outputString = SAKDataStructure::byteArrayToString(inputArray, outputFormat);
    ui->textBrowser->setText(outputString);
}

void SAKStringAssistant::onOutputFormatComboBoxCurrentTextChanged(const QString& text)
{
    Q_UNUSED(text);
    onCreatePushButtonClicked();
}
