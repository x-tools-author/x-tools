/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QDebug>
#include <QRegularExpressionValidator>

#include "SAKCommonInterface.hh"
#include "SAKCommonDataStructure.hh"
#include "SAKOtherAnalyzerThread.hh"
#include "SAKOtherAnalyzerThreadManager.hh"

#include "ui_SAKOtherAnalyzerThreadManager.h"

SAKOtherAnalyzerThreadManager::SAKOtherAnalyzerThreadManager(QSettings *settings, QWidget *parent)
    :QWidget(parent)
    ,mSettingKeyFixed(QString("DebugPage/protocolAnalyzerFixed"))
    ,mSettingKeyLenth(QString("DebugPage/protocolAnalyzerLength"))
    ,mSettingKeyStartBytes(QString("DebugPage/protocolAnalyzerStartBytes"))
    ,mSettingKeyEndBytes(QString("DebugPage/protocolAnalyzerEndBytes"))
    ,mSettingKeyEnable(QString("DebugPage/protocolAnalyzerEnable"))
    ,mSettings(settings)
    ,mAnalyzer(new SAKOtherAnalyzerThread)
    ,mUi(new Ui::SAKOtherAnalyzerThreadManager)
{
    mUi->setupUi(this);
    mFixedLengthCheckBox = mUi->fixedLengthCheckBox;
    mLengthLineEdit = mUi->lengthLineEdit;
    mStartLineEdit = mUi->startLineEdit;
    mEndLineEdit = mUi->endLineEdit;
    mDisableCheckBox = mUi->disableCheckBox;
    mClearPushButton = mUi->clearPushButton;

    auto blockUiSignals = [=](bool block){
        mFixedLengthCheckBox->blockSignals(block);
        mLengthLineEdit->blockSignals(block);
        mStartLineEdit->blockSignals(block);
        mEndLineEdit->blockSignals(block);
    };

    blockUiSignals(true);
    SAKCommonInterface::setLineEditValidator(mStartLineEdit, SAKCommonInterface::ValidatorHex);
    SAKCommonInterface::setLineEditValidator(mEndLineEdit, SAKCommonInterface::ValidatorHex);
    connect(mAnalyzer, &SAKOtherAnalyzerThread::bytesAnalyzed, this, &SAKOtherAnalyzerThreadManager::bytesAnalysed);
    mAnalyzer->start();

    Q_ASSERT_X(settings, __FUNCTION__, "The parameter can not be Q_NULLPTR!");
    if (settings){
        // Read in parameters from settings file
        bool fixed = settings->value(mSettingKeyFixed).toBool();
        QString lengthString = settings->value(mSettingKeyLenth).toString();
        QString startBytesString = settings->value(mSettingKeyStartBytes).toString();
        QString endBytesString = settings->value(mSettingKeyEndBytes).toString();
        bool enable = settings->value(mSettingKeyEnable).toBool();

        mFixedLengthCheckBox->setChecked(fixed);
        mLengthLineEdit->setText(lengthString);
        mStartLineEdit->setText(startBytesString);
        mEndLineEdit->setText(endBytesString);
        mDisableCheckBox->setChecked(!enable);

        // Set parameters
        mAnalyzer->setFixed(fixed);
        mAnalyzer->setLength(lengthString.toInt());
        mAnalyzer->setStartArray(string2bytes(startBytesString));
        mAnalyzer->setEndArray(string2bytes(endBytesString));
        mAnalyzer->setEnable(enable);
    }

    blockUiSignals(false);
}

SAKOtherAnalyzerThreadManager::~SAKOtherAnalyzerThreadManager()
{
    delete mUi;
    delete mAnalyzer;
}

void SAKOtherAnalyzerThreadManager::inputBytes(QByteArray bytes)
{
    mAnalyzer->inputBytes(bytes);
}

QByteArray SAKOtherAnalyzerThreadManager::string2bytes(QString hex)
{
    return SAKCommonDataStructure::stringToByteArray(hex, SAKCommonDataStructure::InputFormatHex);
}

void SAKOtherAnalyzerThreadManager::on_fixedLengthCheckBox_clicked()
{
    mAnalyzer->setFixed(mFixedLengthCheckBox->isChecked());
    if (mSettings){
        mSettings->setValue(mSettingKeyFixed, mFixedLengthCheckBox->isChecked());
    }
}

void SAKOtherAnalyzerThreadManager::on_lengthLineEdit_textChanged(const QString &text)
{
    mAnalyzer->setLength(text.toInt());
    if (mSettings){
        mSettings->setValue(mSettingKeyLenth, mLengthLineEdit->text());
    }
}

void SAKOtherAnalyzerThreadManager::on_startLineEdit_textChanged(const QString &text)
{
    QByteArray startBytes = string2bytes(text);
    mAnalyzer->setStartArray(startBytes);
    if (mSettings){
        mSettings->setValue(mSettingKeyStartBytes, mStartLineEdit->text());
    }
}

void SAKOtherAnalyzerThreadManager::on_endLineEdit_textChanged(const QString &text)
{
    QByteArray endBytes = string2bytes(text);
    mAnalyzer->setEndArray(endBytes);
    if (mSettings){
        mSettings->setValue(mSettingKeyEndBytes, mEndLineEdit->text());
    }
}

void SAKOtherAnalyzerThreadManager::on_disableCheckBox_clicked()
{
    mAnalyzer->setEnable(!mDisableCheckBox->isChecked());
    if (mSettings){
        mSettings->setValue(mSettingKeyEnable, !mDisableCheckBox->isChecked());
    }
}

void SAKOtherAnalyzerThreadManager::on_clearPushButton_clicked()
{
    mAnalyzer->clearData();
}
