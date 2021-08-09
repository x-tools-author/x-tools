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
    connect(&mWriteTimer, &QTimer::timeout,
            this, &SAKDebuggerPluginRegularlySendingItem::write);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mTextFormatComboBox);

    connect(mUi->enableCheckBox, &QCheckBox::click,
            this, [&](){
        mEnableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
    });

    connect(mUi->intervalLineEdit, &QLineEdit::textEdited,
            this, [&](const QString &text){
        if (!isInitializing){
            int interval = text.toInt();
            mWriteTimer.setInterval(interval < 20 ? 20 : interval);
            emit intervalChanged(mID, interval);
        }
    });

    connect(mUi->textFormatComboBox,
            QOverload<int>::of(&QComboBox::currentIndexChanged),
            this,
            [&](int index){
        Q_UNUSED(index);
        if (!isInitializing){
            mInputDataTextEdit->clear();
            int format = mTextFormatComboBox->currentData().toInt();
            emit formatChanged(mID, format);
        }
    });

    connect(mUi->descriptionLineEdit, &QLineEdit::textChanged,
            this, [&](const QString &text){
        if (!isInitializing){
            emit descriptionChanged(mID, text);
        }
    });

    connect(mUi->inputDataTextEdit, &QTextEdit::textChanged,
            this, [&](const QString &text){
        if (!isInitializing){
            int format = mTextFormatComboBox->currentData().toInt();
            SAKCommonDataStructure::formattingInputText(mInputDataTextEdit, format);

            emit inputTextChanged(mID, text);
        }
    });
}
