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
#include "ui_SAKDebuggerPluginRegularlySendingItem.h"

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

    mUi->intervalLineEdit->setText(QString::number(ctx.interval));
    mUi->textFormatComboBox->setCurrentIndex(ctx.format);
    mUi->descriptionLineEdit->setText(ctx.description);
    mUi->inputDataTextEdit->setText(ctx.data);
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
    return mUi->intervalLineEdit->text().toUInt();
}

quint32 SAKDebuggerPluginRegularlySendingItem::itemFormat()
{
    return mUi->textFormatComboBox->currentIndex();
}

QString SAKDebuggerPluginRegularlySendingItem::itemDescription()
{
    return mUi->descriptionLineEdit->text();
}

QString SAKDebuggerPluginRegularlySendingItem::itemText()
{
    return mUi->inputDataTextEdit->toPlainText();
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
    QString data = mUi->inputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = mUi->textFormatComboBox->currentData().toInt();
        emit invokeWriteBytes(data, textFormat);
    }
    mWriteTimer.start();
}

void SAKDebuggerPluginRegularlySendingItem::commonInitializing()
{
    mUi->setupUi(this);
    mWriteTimer.setInterval(mUi->intervalLineEdit->text().toInt());
    connect(&mWriteTimer, &QTimer::timeout,
            this, &SAKDebuggerPluginRegularlySendingItem::write);
    SAKCommonDataStructure::setComboBoxTextInputFormat(mUi->textFormatComboBox);

    connect(mUi->enableCheckBox, &QCheckBox::clicked,
            this, [&](){
        mUi->enableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
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
            mUi->inputDataTextEdit->clear();
            int format = mUi->textFormatComboBox->currentData().toInt();
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
            this, [&](){
        if (!isInitializing){
            QString text = mUi->inputDataTextEdit->toPlainText();
            int format = mUi->textFormatComboBox->currentData().toInt();
            SAKCommonDataStructure::formattingInputText(mUi->inputDataTextEdit,
                                                        format);

            emit inputTextChanged(mID, text);
        }
    });
}
