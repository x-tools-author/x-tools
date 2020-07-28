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

#include "SAKGlobal.hh"
#include "SAKDebugPage.hh"
#include "SAKDataStruct.hh"
#include "SAKOtherTimingSentItem.hh"
#include "SAKDebugPageCommonDatabaseInterface.hh"

#include "ui_SAKOtherTimingSentItem.h"

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,mUi(new Ui::SAKOtherTimingSentItem)
{
    commonInitializing();
    mID = QDateTime::currentMSecsSinceEpoch();
}

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage,
                                               quint64 id,
                                               quint32 interval,
                                               quint32 format,
                                               QString comment,
                                               QString data,
                                               QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,mID(id)
    ,mUi(new Ui::SAKOtherTimingSentItem)
{
    commonInitializing();

    mIntervalLineEdit->setText(QString::number(interval));
    mTextFormatComboBox->setCurrentIndex(format);
    mDescriptionLineEdit->setText(comment);
    mInputDataTextEdit->setText(data);
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
    QString data = mInputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->mTextFormatComboBox->currentData().toInt();
        mDebugPage->writeRawData(data, textFormat);
    }
}

void SAKOtherTimingSentItem::commonInitializing()
{
    mUi->setupUi(this);
    mDatabaseInterface = SAKDebugPageCommonDatabaseInterface::instance();

    mEnableCheckBox = mUi->enableCheckBox;
    mIntervalLineEdit = mUi->intervalLineEdit;
    mTextFormatComboBox = mUi->textFormatComboBox;
    mDescriptionLineEdit = mUi->descriptionLineEdit;
    mInputDataTextEdit = mUi->inputDataTextEdit;

    mWriteTimer.setInterval(mIntervalLineEdit->text().toInt());
    connect(&mWriteTimer, &QTimer::timeout, this, &SAKOtherTimingSentItem::write);

    SAKGlobal::initInputTextFormatComboBox(mTextFormatComboBox);
}

void SAKOtherTimingSentItem::on_enableCheckBox_clicked()
{
    if (mEnableCheckBox){
        mEnableCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
    }
}

void SAKOtherTimingSentItem::on_intervalLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    mWriteTimer.setInterval(interval == 0 ? 1000 : interval);
}

void SAKOtherTimingSentItem::on_textFormatComboBox_currentTextChanged(const QString &text)
{

}

void SAKOtherTimingSentItem::on_descriptionLineEdit_textChanged(const QString &text)
{

}

void SAKOtherTimingSentItem::on_inputDataTextEdit_textChanged()
{

}
