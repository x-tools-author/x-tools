/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMenu>
#include <QDebug>
#include <QSqlError>

#include "SAKDebugPage.hh"
#include "SAKInputDataFactory.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebugPageInputController.hh"

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

SAKInputDataPresetItem::SAKInputDataPresetItem(quint64 id,
                                               quint32 format,
                                               QString description,
                                               QString text,
                                               QWidget *parent)
    :QWidget(parent)
    ,mItemID(id)
    ,mInitializing(true)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializingItem();
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(description);
    mInputTextEdit->setText(text);
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

void SAKInputDataPresetItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    if (!mInitializing){
        mInputTextEdit->clear();

        Q_UNUSED(text);
        int format = mTextFormatComboBox->currentData().toInt();
        emit formatChanged(format);
    }
}

void SAKInputDataPresetItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    if (!mInitializing){
        emit descriptionChanged(text);
    }
}

void SAKInputDataPresetItem::on_inputTextEdit_textChanged()
{
    if (!mInitializing){
        QString text = mInputTextEdit->toPlainText();
        int format = mTextFormatComboBox->currentData().toInt();
        SAKDebugPageInputController::formattingInputText(mInputTextEdit, format);

        emit textChanged(text);
    }
}
