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

#include "ui_SAKInputDataPresetItem.h"

SAKInputDataPresetItem::SAKInputDataPresetItem(QWidget *parent)
    :QWidget(parent)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    mUi->setupUi(this);
    mItemID = QDateTime::currentMSecsSinceEpoch();
}

SAKInputDataPresetItem::SAKInputDataPresetItem(quint64 id,
                                               quint32 format,
                                               QString description,
                                               QString text,
                                               QWidget *parent)
    :QWidget(parent)
    ,mItemID(id)
    ,mUi(new Ui::SAKInputDataPresetItem)
{
    mUi->setupUi(this);
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(description);
    mInputTextEdit->setText(text);
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

void SAKInputDataPresetItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{
    Q_UNUSED(text);
    int format = mTextFormatComboBox->currentData().toInt();
    emit formatChanged(format);
}

void SAKInputDataPresetItem::on_descriptionLineEdit_textChanged(const QString &text)
{
    emit descriptionChanged(text);
}

void SAKInputDataPresetItem::on_inputTextEdit_textChanged()
{
    QString text = mInputTextEdit->toPlainText();
    emit textChanged(text);
}
