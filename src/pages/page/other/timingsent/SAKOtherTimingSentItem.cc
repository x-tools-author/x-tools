/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QDateTime>

#include "SAKDebugPage.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKOtherTimingSentItem.hh"
#include "SAKDebugPageInputController.hh"

#include "ui_SAKOtherTimingSentItem.h"

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,isInitializing(true)
    ,mUi(new Ui::SAKOtherTimingSentItem)
{
    commonInitializing();
    mID = QDateTime::currentMSecsSinceEpoch();
    isInitializing = false;
}

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage,
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
    ,mUi(new Ui::SAKOtherTimingSentItem)
{
    commonInitializing();

    mIntervalLineEdit->setText(QString::number(interval));
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(description);
    mInputDataTextEdit->setText(text);
    isInitializing = false;
}

SAKOtherTimingSentItem::~SAKOtherTimingSentItem()
{
    delete mUi;
}

quint64 SAKOtherTimingSentItem::itemID()
{
    return mID;
}

quint32 SAKOtherTimingSentItem::itemInterval()
{
    return mIntervalLineEdit->text().toUInt();
}

quint32 SAKOtherTimingSentItem::itemFormat()
{
    return mTextFormatComboBox->currentIndex();
}

QString SAKOtherTimingSentItem::itemDescription()
{
    return mDescriptionLineEdit->text();
}

QString SAKOtherTimingSentItem::itemText()
{
    return mInputDataTextEdit->toPlainText();
}

void SAKOtherTimingSentItem::write()
{
    mWriteTimer.stop();
    QString data = mInputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->mTextFormatComboBox->currentData().toInt();
        mDebugPage->writeRawData(data, textFormat);
    }
    mWriteTimer.start();
}

void SAKOtherTimingSentItem::commonInitializing()
{
    mUi->setupUi(this);

    mEnableCheckBox = mUi->enableCheckBox;
    mIntervalLineEdit = mUi->intervalLineEdit;
    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputDataTextEdit = mUi->inputDataTextEdit;

    mWriteTimer.setInterval(mIntervalLineEdit->text().toInt());
    connect(&mWriteTimer, &QTimer::timeout, this, &SAKOtherTimingSentItem::write);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mTextFormatComboBox);
}

void SAKOtherTimingSentItem::on_enableCheckBox_clicked()
{
    if (mEnableCheckBox){
        mEnableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
    }
}

void SAKOtherTimingSentItem::on_intervalLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        int interval = text.toInt();
        mWriteTimer.setInterval(interval < 20 ? 20 : interval);
        emit intervalChanged(interval);
    }
}

void SAKOtherTimingSentItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    if (!isInitializing){
        mInputDataTextEdit->clear();
        int format = mTextFormatComboBox->currentData().toInt();
        emit formatChanged(format);
    }
}

void SAKOtherTimingSentItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    if (!isInitializing){
        emit descriptionChanged(text);
    }
}

void SAKOtherTimingSentItem::on_inputDataTextEdit_textChanged()
{
    if (!isInitializing){
        QString text = mInputDataTextEdit->toPlainText();
        int format = mTextFormatComboBox->currentData().toInt();
        SAKDebugPageInputController::formattingInputText(mInputDataTextEdit, format);

        emit inputTextChanged(text);
    }
}
