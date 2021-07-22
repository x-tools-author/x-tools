/******************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include <QMenu>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>

#include "SAKDebuggerInput.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKCommonDataStructure.hh"

#include "ui_SAKInputDataPresetItem.h"

SAKInputDataPresetItem::SAKInputDataPresetItem(QWidget *parent)
    :QWidget(parent)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializingItem();
    mItemId = QDateTime::currentMSecsSinceEpoch();
}

SAKInputDataPresetItem::SAKInputDataPresetItem(
        SAKStructDataPresetItemContext context,
        QWidget *parent)
    :QWidget(parent)
    ,mItemId(context.id)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializingItem();
    mTextFormatComboBox->setCurrentIndex(context.format);
    mDescriptionLineEdit->setText(context.description);
    mInputTextEdit->setText(context.text);
}

SAKInputDataPresetItem::~SAKInputDataPresetItem()
{
    delete mUi;
}

quint64 SAKInputDataPresetItem::itemID()
{
    return mItemId;
}

QString SAKInputDataPresetItem::itemDescription()
{
    return mDescriptionLineEdit->text();
}

QString SAKInputDataPresetItem::itemText()
{
    return mInputTextEdit->toPlainText();
}

int SAKInputDataPresetItem::itemTextFromat()
{
    return mTextFormatComboBox->currentData().toInt();
}

void SAKInputDataPresetItem::initializingItem()
{
    mUi->setupUi(this);
    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputTextEdit = mUi->inputTextEdit;
    SAKCommonDataStructure::setComboBoxTextInputFormat(mTextFormatComboBox);


    connect(mTextFormatComboBox, &QComboBox::currentTextChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        mInputTextEdit->clear();
        int format = mTextFormatComboBox->currentData().toInt();
        emit formatChanged(mItemId, format);
    });


    connect(mDescriptionLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        emit descriptionChanged(mItemId, text);
    });


    connect(mInputTextEdit, &QTextEdit::textChanged,
            this, [=](){
        QString text = mInputTextEdit->toPlainText();
        int format = mTextFormatComboBox->currentData().toInt();
        SAKDebuggerInput::formattingInputText(mInputTextEdit, format);
        emit textChanged(mItemId, text);
    });
}
