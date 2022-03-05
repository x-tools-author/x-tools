/****************************************************************************************
 * Copyright 2018-2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMenu>
#include <QDebug>
#include <QSqlError>
#include <QDateTime>

#include "SAKDebuggerInput.hh"
#include "SAKCommonCrcInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKDebuggerInputDataPresetItem.hh"
#include "SAKCommonDataStructure.hh"

#include "ui_SAKDebuggerInputDataPresetItem.h"

SAKDebuggerInputDataPresetItem::SAKDebuggerInputDataPresetItem(QWidget *parent)
    :SAKBaseListWidgetItemWidget(parent)
    ,mUi(new Ui::SAKDebuggerInputDataPresetItem)
{
    mUi->setupUi(this);
    blockUiComponentsSignals(true);
    initializingItem();
    blockUiComponentsSignals(false);
}

SAKDebuggerInputDataPresetItem::SAKDebuggerInputDataPresetItem(
        SAKStructItemContext context,
        QWidget *parent)
    :SAKBaseListWidgetItemWidget(context.id, parent)
    ,mUi(new Ui::SAKDebuggerInputDataPresetItem)
{
    mUi->setupUi(this);
    blockUiComponentsSignals(true);
    initializingItem();

    mUi->formatComboBox->setCurrentIndex(context.format);
    mUi->descriptionLineEdit->setText(context.description);
    mUi->dataLineEdit->setText(context.data);
    blockUiComponentsSignals(false);

    connect(mUi->sendPushButton, &QPushButton::clicked, this, [=](){
        SAKStructItemContext ctx = this->context();
        QByteArray data = SAKCommonDataStructure::stringToByteArray(
                    ctx.data, ctx.format);
        emit invokeWriteCookedBytes(data);
    });
}

SAKDebuggerInputDataPresetItem::~SAKDebuggerInputDataPresetItem()
{
    delete mUi;
}

SAKDebuggerInputDataPresetItem::SAKStructItemContext
SAKDebuggerInputDataPresetItem::context()
{
    SAKStructItemContext ctx;
    ctx.id = id();
    ctx.description = mUi->descriptionLineEdit->text();
    ctx.format = mUi->formatComboBox->currentData().toInt();
    ctx.data = mUi->dataLineEdit->text();

    return ctx;
}

void SAKDebuggerInputDataPresetItem::initializingItem()
{
    SAKCommonDataStructure::setComboBoxTextInputFormat(
                mUi->formatComboBox
                );
    SAKCommonDataStructure::setLineEditTextFormat(
                mUi->dataLineEdit,
                mUi->formatComboBox->currentData().toInt()
                );


    connect(mUi->formatComboBox, &QComboBox::currentTextChanged,
            this, [=](const QString &text){
        Q_UNUSED(text);
        mUi->dataLineEdit->clear();
        int format = mUi->formatComboBox->currentData().toInt();
        emit formatChanged(id(), format);

        SAKCommonDataStructure::setLineEditTextFormat(
                    mUi->dataLineEdit,
                    mUi->formatComboBox->currentData().toInt()
                    );
    });


    connect(mUi->descriptionLineEdit, &QLineEdit::textChanged,
            this, [=](const QString &text){
        emit descriptionChanged(id(), text);
    });


    connect(mUi->dataLineEdit, &QLineEdit::textChanged,
            this, [=](){
        QString text = mUi->dataLineEdit->text();
        emit dataChanged(id(), text);
    });
}

void SAKDebuggerInputDataPresetItem::blockUiComponentsSignals(bool block)
{
    mUi->descriptionLineEdit->blockSignals(block);
    mUi->formatComboBox->blockSignals(block);
    mUi->dataLineEdit->blockSignals(block);
}
