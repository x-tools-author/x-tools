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

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKDebugPageInputController.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKInputDataPresetItem.h"

SAKInputDataPresetItem::SAKInputDataPresetItem(QSqlDatabase *sqlDatabase, QWidget *parent)
    :QWidget(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initUi();
    mItemID = QDateTime::currentMSecsSinceEpoch();
}

SAKInputDataPresetItem::SAKInputDataPresetItem(quint64 id,
                                               quint32 format,
                                               QString comment,
                                               QString data,
                                               QSqlDatabase *sqlDatabase,
                                               QWidget *parent)
    :QWidget(parent)
    ,mItemID(id)
    ,mSqlDatabase(sqlDatabase)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initUi();
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(comment);
    mInputTextEdit->setText(data);
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

void SAKInputDataPresetItem::initUi()
{
    mUi->setupUi(this);

    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputTextEdit = mUi->inputTextEdit;
    SAKGlobal::initInputTextFormatComboBox(mTextFormatComboBox);
}

void SAKInputDataPresetItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    mInputTextEdit->clear();
    Q_UNUSED(text);
}

void SAKInputDataPresetItem::on_descriptionLineEdit_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
}

void SAKInputDataPresetItem::on_inputTextEdit_currentTextChanged(const QString &text)
{
    // It seems to be not OOP
    SAKDebugPageInputController::formattingInputText(mInputTextEdit, mTextFormatComboBox->currentData().toInt());
    Q_UNUSED(text);
}
