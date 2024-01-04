/***************************************************************************************************
 * Copyright 2020-2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakstringassistant.h"

#include "sakcommondatastructure.h"
#include "ui_sakstringassistant.h"

SAKStringAssistant::SAKStringAssistant(QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::SAKStringAssistant)
{
    ui->setupUi(this);
    SAKCommonDataStructure::setComboBoxTextInputFormat(ui->inputFormatComboBox);
    SAKCommonDataStructure::setComboBoxTextOutputFormat(ui->outputFormatComboBox);

    connect(ui->textEdit,
            &QTextEdit::textChanged,
            this,
            &SAKStringAssistant::OnTextEditTextChanged);
    connect(ui->inputFormatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SAKStringAssistant::OnInputFormatComboBoxCurrentIndexChanged);
    connect(ui->createPushButton,
            &QPushButton::clicked,
            this,
            &SAKStringAssistant::OnCreatePushButtonClicked);
    connect(ui->outputFormatComboBox,
            &QComboBox::currentTextChanged,
            this,
            &SAKStringAssistant::OnOutputFormatComboBoxCurrentTextChanged);
}

SAKStringAssistant::~SAKStringAssistant()
{
    delete ui;
}

void SAKStringAssistant::OnTextEditTextChanged()
{
    if (!ui->textEdit->blockSignals(true)) {
        QString inputString = ui->textEdit->toPlainText();
        auto inputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(
            ui->inputFormatComboBox->currentData().toInt());
        QString cookedString = SAKCommonDataStructure::formattingString(inputString, inputFormat);
        ui->textEdit->setText(cookedString);
        ui->textEdit->moveCursor(QTextCursor::End);
        ui->textEdit->blockSignals(false);
        OnCreatePushButtonClicked();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Oh, No!");
    }
}

void SAKStringAssistant::OnInputFormatComboBoxCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui->textEdit->clear();
    OnCreatePushButtonClicked();
}

void SAKStringAssistant::OnCreatePushButtonClicked()
{
    QString inputString = ui->textEdit->toPlainText();
    auto inputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(
        ui->inputFormatComboBox->currentData().toInt());
    QByteArray inputArray = SAKCommonDataStructure::stringToByteArray(inputString, inputFormat);
    auto outputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatOutput>(
        ui->outputFormatComboBox->currentData().toInt());
    auto outputString = SAKCommonDataStructure::byteArrayToString(inputArray, outputFormat);
    ui->textBrowser->setText(outputString);
}

void SAKStringAssistant::OnOutputFormatComboBoxCurrentTextChanged(const QString& text)
{
    Q_UNUSED(text);
    OnCreatePushButtonClicked();
}
