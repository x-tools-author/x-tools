/***************************************************************************************************
 * Copyright 2020-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "StringAssistant.h"
#include "ui_StringAssistant.h"

#include "IO/xIO.h"

StringAssistant::StringAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::StringAssistant)
{
    ui->setupUi(this);
    connect(ui->textEdit, &QTextEdit::textChanged, this, &StringAssistant::onTextEditTextChanged);
    connect(ui->inputFormatComboBox,
            static_cast<void (QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            &StringAssistant::onInputFormatComboBoxCurrentIndexChanged);
    connect(ui->createPushButton,
            &QPushButton::clicked,
            this,
            &StringAssistant::onCreatePushButtonClicked);
    connect(ui->outputFormatComboBox,
            &QComboBox::currentTextChanged,
            this,
            &StringAssistant::onOutputFormatComboBoxCurrentTextChanged);

    xIO::setupTextFormat(ui->outputFormatComboBox);
    xIO::setupTextFormat(ui->inputFormatComboBox);
}

StringAssistant::~StringAssistant()
{
    delete ui;
}

void StringAssistant::onTextEditTextChanged()
{
    if (!ui->textEdit->blockSignals(true)) {
        QString inputString = ui->textEdit->toPlainText();
        ui->textEdit->setText("to do");
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->blockSignals(false);
        onCreatePushButtonClicked();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Oh, No!");
    }
}

void StringAssistant::onInputFormatComboBoxCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->textEdit->clear();
    onCreatePushButtonClicked();
}

void StringAssistant::onCreatePushButtonClicked()
{
    QString inputString = ui->textEdit->toPlainText();
    auto inputFormat = static_cast<xIO::TextFormat>(ui->inputFormatComboBox->currentData().toInt());
    QByteArray inputArray = xIO::string2bytes(inputString, inputFormat);
    auto outputFormat = static_cast<xIO::TextFormat>(
        ui->outputFormatComboBox->currentData().toInt());
    auto outputString = xIO::bytes2string(inputArray, outputFormat);
    ui->textBrowser->setText(outputString);
}

void StringAssistant::onOutputFormatComboBoxCurrentTextChanged(const QString& text)
{
    Q_UNUSED(text);
    onCreatePushButtonClicked();
}
