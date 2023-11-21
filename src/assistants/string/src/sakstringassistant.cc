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
    , ui_(new Ui::SAKStringAssistant)
{
    ui_->setupUi(this);
    SAKCommonDataStructure::setComboBoxTextInputFormat(ui_->inputFormatComboBox);
    SAKCommonDataStructure::setComboBoxTextOutputFormat(ui_->outputFormatComboBox);

    connect(ui_->textEdit,
            &QTextEdit::textChanged,
            this,
            &SAKStringAssistant::OnTextEditTextChanged);
    connect(ui_->inputFormatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            &SAKStringAssistant::OnInputFormatComboBoxCurrentIndexChanged);
    connect(ui_->createPushButton,
            &QPushButton::clicked,
            this,
            &SAKStringAssistant::OnCreatePushButtonClicked);
    connect(ui_->outputFormatComboBox,
            &QComboBox::currentTextChanged,
            this,
            &SAKStringAssistant::OnOutputFormatComboBoxCurrentTextChanged);
}

SAKStringAssistant::~SAKStringAssistant()
{
    delete ui_;
}

void SAKStringAssistant::OnTextEditTextChanged()
{
    if (!ui_->textEdit->blockSignals(true)) {
        QString inputString = ui_->textEdit->toPlainText();
        auto inputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(
            ui_->inputFormatComboBox->currentData().toInt());
        QString cookedString = SAKCommonDataStructure::formattingString(inputString, inputFormat);
        ui_->textEdit->setText(cookedString);
        ui_->textEdit->moveCursor(QTextCursor::End);
        ui_->textEdit->blockSignals(false);
        OnCreatePushButtonClicked();
    } else {
        Q_ASSERT_X(false, __FUNCTION__, "Oh, No!");
    }
}

void SAKStringAssistant::OnInputFormatComboBoxCurrentIndexChanged(int index)
{
    Q_UNUSED(index);
    ui_->textEdit->clear();
    OnCreatePushButtonClicked();
}

void SAKStringAssistant::OnCreatePushButtonClicked()
{
    QString inputString = ui_->textEdit->toPlainText();
    auto inputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatInput>(
        ui_->inputFormatComboBox->currentData().toInt());
    QByteArray inputArray = SAKCommonDataStructure::stringToByteArray(inputString, inputFormat);
    auto outputFormat = static_cast<SAKCommonDataStructure::SAKEnumTextFormatOutput>(
        ui_->outputFormatComboBox->currentData().toInt());
    auto outputString = SAKCommonDataStructure::byteArrayToString(inputArray, outputFormat);
    ui_->textBrowser->setText(outputString);
}

void SAKStringAssistant::OnOutputFormatComboBoxCurrentTextChanged(const QString& text)
{
    Q_UNUSED(text);
    OnCreatePushButtonClicked();
}
