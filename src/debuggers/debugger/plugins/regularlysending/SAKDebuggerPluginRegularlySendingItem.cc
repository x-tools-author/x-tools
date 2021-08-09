/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QDebug>
#include <QDateTime>

#include "SAKDebuggerInput.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerPluginRegularlySendingItem.hh"

#include "ui_SAKPluginRegularlySendingItem.h"

SAKDebuggerPluginRegularlySendingItem::SAKDebuggerPluginRegularlySendingItem(
        QWidget *parent
        )
    :QWidget(parent)
    ,isInitializing(true)
    ,mUi(new Ui::SAKDebuggerPluginRegularlySendingItem)
{
    commonInitializing();
    mID = QDateTime::currentMSecsSinceEpoch();
    isInitializing = false;
}

SAKDebuggerPluginRegularlySendingItem::SAKDebuggerPluginRegularlySendingItem(
        SAKStructItemContext ctx,
        QWidget *parent
        )
    :QWidget(parent)
    ,mID(ctx.id)
    ,isInitializing(true)
    ,mUi(new Ui::SAKDebuggerPluginRegularlySendingItem)
{
    commonInitializing();

    mIntervalLineEdit->setText(QString::number(ctx.interval));
    mTextFormatComboBox->setCurrentIndex(ctx.format);
    mDescriptionLineEdit->setText(ctx.description);
    mInputDataTextEdit->setText(ctx.data);
    isInitializing = false;
}

SAKDebuggerPluginRegularlySendingItem::~SAKDebuggerPluginRegularlySendingItem()
{
    delete mUi;
}

quint64 SAKDebuggerPluginRegularlySendingItem::itemID()
{
    return mID;
}

quint32 SAKDebuggerPluginRegularlySendingItem::itemInterval()
{
    return mIntervalLineEdit->text().toUInt();
}

quint32 SAKDebuggerPluginRegularlySendingItem::itemFormat()
{
    return mTextFormatComboBox->currentIndex();
}

QString SAKDebuggerPluginRegularlySendingItem::itemDescription()
{
    return mDescriptionLineEdit->text();
}

QString SAKDebuggerPluginRegularlySendingItem::itemText()
{
    return mInputDataTextEdit->toPlainText();
}

SAKDebuggerPluginRegularlySendingItem::SAKStructItemContext
SAKDebuggerPluginRegularlySendingItem::context()
{
    mContext.id = mID;
    mContext.data = mUi->inputDataTextEdit->toPlainText();
    mContext.format = mUi->textFormatComboBox->currentData().toInt();
    mContext.interval = mUi->intervalLineEdit->text().toInt();
    mContext.description = mUi->descriptionLineEdit->text();
    return mContext;
}

void SAKDebuggerPluginRegularlySendingItem::write()
{
    mWriteTimer.stop();
    QString data = mInputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->mTextFormatComboBox->currentData().toInt();
        emit invokeWriteBytes(data, textFormat);
    }
    mWriteTimer.start();
}

void SAKDebuggerPluginRegularlySendingItem::commonInitializing()
{
    mUi->setupUi(this);

    mEnableCheckBox = mUi->enableCheckBox;
    mIntervalLineEdit = mUi->intervalLineEdit;
    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputDataTextEdit = mUi->inputDataTextEdit;

    mWriteTimer.setInterval(mIntervalLineEdit->text().toInt());
    connect(&mWriteTimer, &QTimer::timeout, this, &SAKDebuggerPluginRegularlySendingItem::write);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mTextFormatComboBox);
}

void SAKDebuggerPluginRegularlySendingItem::on_enableCheckBox_clicked()
{
    if (mEnableCheckBox){
        mEnableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
    }
}

void SAKDebuggerPluginRegularlySendingItem::on_intervalLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        int interval = text.toInt();
        mWriteTimer.setInterval(interval < 20 ? 20 : interval);
        emit intervalChanged(mID, interval);
    }
}

void SAKDebuggerPluginRegularlySendingItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    if (!isInitializing){
        mInputDataTextEdit->clear();
        int format = mTextFormatComboBox->currentData().toInt();
        emit formatChanged(mID, format);
    }
}

void SAKDebuggerPluginRegularlySendingItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        emit descriptionChanged(mID, text);
    }
}

void SAKDebuggerPluginRegularlySendingItem::on_inputDataTextEdit_textChanged()
{
    if (!isInitializing){
        QString text = mInputDataTextEdit->toPlainText();
        int format = mTextFormatComboBox->currentData().toInt();
        SAKCommonDataStructure::formattingInputText(mInputDataTextEdit, format);

        emit inputTextChanged(mID, text);
    }
}
