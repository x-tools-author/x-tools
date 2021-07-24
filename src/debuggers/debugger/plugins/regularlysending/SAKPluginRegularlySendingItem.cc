/*
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QDateTime>

#include "SAKDebugger.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKPluginRegularlySendingItem.hh"
#include "SAKDebuggerInput.hh"

#include "ui_SAKPluginRegularlySendingItem.h"

SAKPluginRegularlySendingItem::SAKPluginRegularlySendingItem(SAKDebugger *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,isInitializing(true)
    ,mUi(new Ui::SAKPluginRegularlySendingItem)
{
    commonInitializing();
    mID = QDateTime::currentMSecsSinceEpoch();
    isInitializing = false;
}

SAKPluginRegularlySendingItem::SAKPluginRegularlySendingItem(SAKDebugger *debugPage,
                                               quint64 id,
                                               quint32 interval,
                                               quint32 format,
                                               QString description,
                                               QString text,
                                               QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,mID(id)
    ,isInitializing(true)
    ,mUi(new Ui::SAKPluginRegularlySendingItem)
{
    commonInitializing();

    mIntervalLineEdit->setText(QString::number(interval));
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(description);
    mInputDataTextEdit->setText(text);
    isInitializing = false;
}

SAKPluginRegularlySendingItem::~SAKPluginRegularlySendingItem()
{
    delete mUi;
}

quint64 SAKPluginRegularlySendingItem::itemID()
{
    return mID;
}

quint32 SAKPluginRegularlySendingItem::itemInterval()
{
    return mIntervalLineEdit->text().toUInt();
}

quint32 SAKPluginRegularlySendingItem::itemFormat()
{
    return mTextFormatComboBox->currentIndex();
}

QString SAKPluginRegularlySendingItem::itemDescription()
{
    return mDescriptionLineEdit->text();
}

QString SAKPluginRegularlySendingItem::itemText()
{
    return mInputDataTextEdit->toPlainText();
}

void SAKPluginRegularlySendingItem::write()
{
    mWriteTimer.stop();
    QString data = mInputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->mTextFormatComboBox->currentData().toInt();
        mDebugPage->writeRawData(data, textFormat);
    }
    mWriteTimer.start();
}

void SAKPluginRegularlySendingItem::commonInitializing()
{
    mUi->setupUi(this);

    mEnableCheckBox = mUi->enableCheckBox;
    mIntervalLineEdit = mUi->intervalLineEdit;
    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputDataTextEdit = mUi->inputDataTextEdit;

    mWriteTimer.setInterval(mIntervalLineEdit->text().toInt());
    connect(&mWriteTimer, &QTimer::timeout, this, &SAKPluginRegularlySendingItem::write);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mTextFormatComboBox);
}

void SAKPluginRegularlySendingItem::on_enableCheckBox_clicked()
{
    if (mEnableCheckBox){
        mEnableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
    }
}

void SAKPluginRegularlySendingItem::on_intervalLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        int interval = text.toInt();
        mWriteTimer.setInterval(interval < 20 ? 20 : interval);
        emit intervalChanged(interval);
    }
}

void SAKPluginRegularlySendingItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    if (!isInitializing){
        mInputDataTextEdit->clear();
        int format = mTextFormatComboBox->currentData().toInt();
        emit formatChanged(format);
    }
}

void SAKPluginRegularlySendingItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        emit descriptionChanged(text);
    }
}

void SAKPluginRegularlySendingItem::on_inputDataTextEdit_textChanged()
{
    if (!isInitializing){
        QString text = mInputDataTextEdit->toPlainText();
        int format = mTextFormatComboBox->currentData().toInt();
        SAKCommonDataStructure::formattingInputText(mInputDataTextEdit, format);

        emit inputTextChanged(text);
    }
}
