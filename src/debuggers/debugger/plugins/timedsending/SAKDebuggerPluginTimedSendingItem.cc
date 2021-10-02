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
#include "SAKDebuggerPluginTimedSendingItem.hh"
#include "ui_SAKDebuggerPluginTimedSendingItem.h"

SAKDebuggerPluginTimedSendingItem::SAKDebuggerPluginTimedSendingItem(
        QWidget *parent
        )
    :SAKBaseListWidgetItemWidget(parent)
    ,mUi(new Ui::SAKDebuggerPluginTimedSendingItem)
{
    mUi->setupUi(this);
    blockUiCommpentsSignals(true);
    commonInitializing();
    blockUiCommpentsSignals(false);
}

SAKDebuggerPluginTimedSendingItem::SAKDebuggerPluginTimedSendingItem(
        SAKStructItemContext ctx,
        QWidget *parent
        )
    :SAKBaseListWidgetItemWidget(parent)
    ,mUi(new Ui::SAKDebuggerPluginTimedSendingItem)
{
    mUi->setupUi(this);
    blockUiCommpentsSignals(true);
    commonInitializing();
    mUi->enableCheckBox->setChecked(ctx.enable);
    mUi->descriptionLineEdit->setText(ctx.description);
    mUi->intervalSpinBox->setValue(ctx.interval);
    mUi->textFormatComboBox->setCurrentIndex(ctx.format);
    mUi->suffixComboBox->setCurrentIndex(ctx.suffix);
    mUi->dataLineEdit->setText(ctx.data);

    mUi->suffixComboBox->setCurrentIndex(ctx.suffix);
    SAKCommonDataStructure::setLineEditTextFormat(mUi->dataLineEdit, ctx.format);
    blockUiCommpentsSignals(false);

    if (ctx.enable) {
        mWriteTimer.start();
    }
}

SAKDebuggerPluginTimedSendingItem::~SAKDebuggerPluginTimedSendingItem()
{
    delete mUi;
}

const SAKDebuggerPluginTimedSendingItem::SAKStructItemContext
SAKDebuggerPluginTimedSendingItem::context()
{
    mContext.id = id();
    mContext.enable = mUi->enableCheckBox->isChecked();
    mContext.description = mUi->descriptionLineEdit->text();
    mContext.interval = mUi->intervalSpinBox->text().toInt();
    mContext.format = mUi->textFormatComboBox->currentData().toInt();
    mContext.suffix = mUi->suffixComboBox->currentData().toInt();
    mContext.data = mUi->dataLineEdit->text();

    return mContext;
}

void SAKDebuggerPluginTimedSendingItem::write()
{
    mWriteTimer.stop();

    QString data = mUi->dataLineEdit->text();
    if (!data.isEmpty()){
        QString text = mUi->dataLineEdit->text();
        int textFormat = mUi->textFormatComboBox->currentData().toInt();
        int suffix = mUi->suffixComboBox->currentData().toInt();

        QByteArray data = SAKCommonDataStructure::stringToByteArray(text, textFormat);
        QByteArray suffixArray = SAKCommonDataStructure::suffix(suffix).toLatin1();
        data.append(suffixArray);

        emit invokeWriteCookedBytes(data);
    }

    if (mUi->enableCheckBox->isChecked()) {
        mWriteTimer.start();
    }
}

void SAKDebuggerPluginTimedSendingItem::commonInitializing()
{
    connect(&mWriteTimer, &QTimer::timeout,
            this, &SAKDebuggerPluginTimedSendingItem::write);

    connect(mUi->enableCheckBox, &QCheckBox::clicked,
            this, [&](){
        mUi->enableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
        emit enableChanged(id(), mUi->enableCheckBox->isChecked());
    });

    connect(mUi->descriptionLineEdit, &QLineEdit::textChanged,
            this, [&](const QString &text){
        emit descriptionChanged(id(), text);
    });

    connect(mUi->intervalSpinBox,
            static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this, [&](int value){
        mWriteTimer.setInterval(value < 50 ? 50 : value);
        emit intervalChanged(id(), value);
    });

    connect(mUi->textFormatComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            [&](int index){
        Q_UNUSED(index);
        mUi->dataLineEdit->clear();
        int format = mUi->textFormatComboBox->currentData().toInt();
        SAKCommonDataStructure::setLineEditTextFormat(mUi->dataLineEdit, format);
        emit formatChanged(id(), format);
    });

    connect(mUi->suffixComboBox,
            static_cast<void(QComboBox::*)(int)>(&QComboBox::currentIndexChanged),
            this,
            [&](int index){
        emit suffixChanged(id(), index);
    });

    connect(mUi->dataLineEdit, &QLineEdit::textChanged,
            this, [&](){
        emit dataChanged(id(), mUi->dataLineEdit->text());
    });

    SAKCommonDataStructure::setComboBoxTextInputFormat(mUi->textFormatComboBox);
    SAKCommonDataStructure::setupSuffix(mUi->suffixComboBox);
    SAKCommonDataStructure::setLineEditTextFormat(
                mUi->dataLineEdit,
                mUi->textFormatComboBox->currentIndex()
                );

    mUi->enableCheckBox->setChecked(false);
    mWriteTimer.setInterval(mUi->intervalSpinBox->value());
    mWriteTimer.setSingleShot(true);
}

void SAKDebuggerPluginTimedSendingItem::blockUiCommpentsSignals(bool block)
{
    mUi->enableCheckBox->blockSignals(block);
    mUi->intervalSpinBox->blockSignals(block);
    mUi->descriptionLineEdit->blockSignals(block);
    mUi->textFormatComboBox->blockSignals(block);
    mUi->suffixComboBox->blockSignals(block);
    mUi->dataLineEdit->blockSignals(block);
}
