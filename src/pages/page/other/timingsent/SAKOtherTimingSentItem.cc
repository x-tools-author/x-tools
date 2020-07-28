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

#include "ui_SAKOtherTimingSentItem.h"

SAKOtherTimingSentItem::SAKOtherTimingSentItem(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,mDebugPage(debugPage)
    ,ui(new Ui::SAKOtherTimingSentItem)
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
    ,ui(new Ui::SAKOtherTimingSentItem)
{
    commonInitializing();

    timingTimeLineEdit->setText(QString::number(interval));
    textFormatComboBox->setCurrentIndex(format);
    descriptionLineEdit->setText(comment);
    inputDataTextEdit->setText(data);
}

SAKOtherTimingSentItem::~SAKOtherTimingSentItem()
{
    delete ui;
}

quint64 SAKOtherTimingSentItem::itemID()
{
    return mID;
}

quint32 SAKOtherTimingSentItem::itemInterval()
{
    return timingTimeLineEdit->text().toUInt();
}

quint32 SAKOtherTimingSentItem::itemFormat()
{
    return textFormatComboBox->currentIndex();
}

QString SAKOtherTimingSentItem::itemDescription()
{
    return descriptionLineEdit->text();
}

QString SAKOtherTimingSentItem::itemText()
{
    return inputDataTextEdit->toPlainText();
}

void SAKOtherTimingSentItem::write()
{
    QString data = inputDataTextEdit->toPlainText();

    if (!data.isEmpty()){
        int textFormat = this->textFormatComboBox->currentData().toInt();
        mDebugPage->writeRawData(data, textFormat);
    }
}

void SAKOtherTimingSentItem::commonInitializing()
{
    ui->setupUi(this);

    timingCheckBox = ui->timingCheckBox;
    timingTimeLineEdit = ui->timingTimeLineEdit;
    textFormatComboBox = ui->textFormatComboBox;
    descriptionLineEdit = ui->descriptionLineEdit;
    inputDataTextEdit = ui->inputDataTextEdit;
    updatePushButton = ui->updatePushButton;

    mWriteTimer.setInterval(timingTimeLineEdit->text().toInt());
    connect(&mWriteTimer, &QTimer::timeout, this, &SAKOtherTimingSentItem::write);

    SAKGlobal::initInputTextFormatComboBox(textFormatComboBox);
}

void SAKOtherTimingSentItem::on_timingCheckBox_clicked()
{
    if (timingCheckBox){
        timingCheckBox->isChecked() ? mWriteTimer.start() : mWriteTimer.stop();
    }
}

void SAKOtherTimingSentItem::on_timingTimeLineEdit_textChanged(const QString &text)
{
    int interval = text.toInt();
    mWriteTimer.setInterval(interval == 0 ? 1000 : interval);
}

void SAKOtherTimingSentItem::on_updatePushButton_clicked()
{
    QString tableName = SAKDataStruct::timingSendingTableName(mDebugPage->pageType());
    SAKDataStruct::SAKStructTimingSendingItem sendingItem;
    sendingItem.id = itemID();
    sendingItem.data = itemText();
    sendingItem.format = itemFormat();
    sendingItem.comment = itemDescription();
    sendingItem.interval = itemInterval();
//    SAKDebugPageDatabaseInterface *databaseInterface = SAKDebugPageDatabaseInterface::instance();
//    databaseInterface->updateTimingSendingItem(tableName, sendingItem);
}
