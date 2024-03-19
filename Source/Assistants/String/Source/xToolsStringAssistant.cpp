/***************************************************************************************************
 * Copyright 2020-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsStringAssistant.h"
#include "ui_xToolsStringAssistant.h"

#include "sakdatastructure.h"

xToolsStringAssistant::xToolsStringAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::xToolsStringAssistant)
{
    ui->setupUi(this);
    SAKDataStructure::setComboBoxTextInputFormat(ui->inputFormatComboBox);
    SAKDataStructure::setComboBoxTextOutputFormat(ui->outputFormatComboBox);

    connect(ui->textEdit, &QTextEdit::textChanged, this, &xToolsStringAssistant::onTextEditTextChanged);
    connect(ui->inputFormatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &xToolsStringAssistant::onInputFormatComboBoxCurrentIndexChanged);
    connect(ui->createPushButton,
            &QPushButton::clicked,
            this,
            &xToolsStringAssistant::onCreatePushButtonClicked);
    connect(ui->outputFormatComboBox,
            &QComboBox::currentTextChanged,
            this,
            &xToolsStringAssistant::onOutputFormatComboBoxCurrentTextChanged);
}

xToolsStringAssistant::~xToolsStringAssistant()
{
    delete ui;
}

void xToolsStringAssistant::onTextEditTextChanged()
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

void xToolsStringAssistant::onInputFormatComboBoxCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->textEdit->clear();
    onCreatePushButtonClicked();
}

void xToolsStringAssistant::onCreatePushButtonClicked()
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

void xToolsStringAssistant::onOutputFormatComboBoxCurrentTextChanged(const QString& text)
{
    Q_UNUSED(text);
    onCreatePushButtonClicked();
}
