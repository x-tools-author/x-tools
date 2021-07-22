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
    ,mInitializing(true)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializingItem();
    mItemID = QDateTime::currentMSecsSinceEpoch();
    mInitializing = false;
}

SAKInputDataPresetItem::SAKInputDataPresetItem(
        SAKStructDataPresetItemContext context,
        QWidget *parent
        )
    :QWidget(parent)
    ,mItemID(context.id)
    ,mInitializing(true)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializingItem();
    mTextFormatComboBox->setCurrentIndex(context.format);
    mDescriptionLineEdit->setText(context.description);
    mInputTextEdit->setText(context.text);
    mInitializing = false;
}

SAKInputDataPresetItem::~SAKInputDataPresetItem()
{
    delete mUi;
}

quint64 SAKInputDataPresetItem::itemID()
{
    return mItemID;
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
}

void SAKInputDataPresetItem::on_textFormatComboBox_currentTextChanged(
        const QString &text
        )
{
    if (!mInitializing){
        mInputTextEdit->clear();

        Q_UNUSED(text);
        int format = mTextFormatComboBox->currentData().toInt();
        emit formatChanged(mItemID, format);
    }
}

void SAKInputDataPresetItem::on_descriptionLineEdit_textChanged(
        const QString &text
        )
{
    if (!mInitializing){
        emit descriptionChanged(mItemID, text);
    }
}

void SAKInputDataPresetItem::on_inputTextEdit_textChanged()
{
    if (!mInitializing){
        QString text = mInputTextEdit->toPlainText();
        int format = mTextFormatComboBox->currentData().toInt();
        SAKDebuggerInput::formattingInputText(mInputTextEdit, format);

        emit textChanged(mItemID, text);
    }
}
