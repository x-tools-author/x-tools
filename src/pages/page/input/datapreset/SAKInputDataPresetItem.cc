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

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKCRCInterface.hh"
#include "SAKInputDataFactory.hh"
#include "SAKInputDataPresetItem.hh"
#include "SAKDebugPageInputController.hh"
#include "SAKDebugPageDatabaseInterface.hh"

#include "ui_SAKInputDataPresetItem.h"

SAKInputDataPresetItem::SAKInputDataPresetItem(int pageType, QSqlDatabase *sqlDatabase, QWidget *parent)
    :QWidget(parent)
    ,mPageType(pageType)
    ,mSqlDatabase(sqlDatabase)
    ,mSqlQuery(Q_NULLPTR)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializeVariable();
    mItemID = QDateTime::currentMSecsSinceEpoch();
}

SAKInputDataPresetItem::SAKInputDataPresetItem(quint64 id,
                                               quint32 format,
                                               QString comment,
                                               QString text,
                                               int pageType,
                                               QSqlDatabase *sqlDatabase,
                                               QWidget *parent)
    :QWidget(parent)
    ,mItemID(id)
    ,mPageType(pageType)
    ,mSqlDatabase(sqlDatabase)
    ,mSqlQuery(Q_NULLPTR)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    initializeVariable();
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(comment);
    mInputTextEdit->setText(text);
}

SAKInputDataPresetItem::~SAKInputDataPresetItem()
{
    delete mUi;
    if (mSqlQuery){
        delete mSqlQuery;
    }
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

void SAKInputDataPresetItem::initializeVariable()
{
    mUi->setupUi(this);

    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputTextEdit = mUi->inputTextEdit;
    SAKGlobal::initInputTextFormatComboBox(mTextFormatComboBox);

    // tale name fo database
    mTableName = SAKDataStruct::presettingDataTableName(mPageType);

    if (mSqlDatabase){
        mSqlQuery = new QSqlQuery(*mSqlDatabase);
    }
}

void SAKInputDataPresetItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    mInputTextEdit->clear();
    Q_UNUSED(text);
}

void SAKInputDataPresetItem::on_descriptionLineEdit_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    emit descriptionChanged(text.length() ? text : tr("Empty"));
}

void SAKInputDataPresetItem::on_inputTextEdit_currentTextChanged(const QString &text)
{
    // It seems to be not OOP
    SAKDebugPageInputController::formattingInputText(mInputTextEdit, mTextFormatComboBox->currentData().toInt());

    // update record
    if (mTableName.length() && mSqlDatabase){
        DatabaseColumns columns;
        const QString queryString = QString("UPDATE %1 SET %2='%3' WHERE %4=%5")
                .arg(mTableName)
                .arg(columns.text)
                .arg(text)
                .arg(columns.id)
                .arg(mItemID);
        if(!mSqlQuery->exec(queryString)){
            qWarning() << __FUNCTION__ << "Can not update record:" << mSqlQuery->lastError().text();
        }
    }
}
