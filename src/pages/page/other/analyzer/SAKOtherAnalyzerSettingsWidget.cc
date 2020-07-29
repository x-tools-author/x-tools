/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDebug>
#include <QRegExpValidator>

#include "SAKOtherAnalyzerThread.hh"
#include "SAKOtherAnalyzerSettingsWidget.hh"

#include "ui_SAKProtocolAnalyzerWidget.h"

SAKOtherAnalyzerSettingsWidget::SAKOtherAnalyzerSettingsWidget(QSettings *settings, QWidget *parent)
    :QWidget(parent)
    ,mSettingKeyFixed(QString("DebugPage/protocolAnalyzerFixed"))
    ,mSettingKeyLenth(QString("DebugPage/protocolAnalyzerLength"))
    ,mSettingKeyStartBytes(QString("DebugPage/protocolAnalyzerStartBytes"))
    ,mSettingKeyEndBytes(QString("DebugPage/protocolAnalyzerEndBytes"))
    ,mSettingKeyEnable(QString("DebugPage/protocolAnalyzerEnable"))
    ,mSettings(settings)
    ,mAnalyzer(new SAKOtherAnalyzerThread)
    ,mUi(new Ui::SAKProtocolAnalyzerWidget)
{
    mUi->setupUi(this);
    mFixedLengthCheckBox = mUi->fixedLengthCheckBox;
    mLengthLineEdit = mUi->lengthLineEdit;
    mStartLineEdit = mUi->startLineEdit;
    mEndLineEdit = mUi->endLineEdit;
    mDisableCheckBox = mUi->disableCheckBox;
    mClearPushButton = mUi->clearPushButton;

    setLineEditFormat(mStartLineEdit);
    setLineEditFormat(mEndLineEdit);
    connect(mAnalyzer, &SAKOtherAnalyzerThread::bytesAnalized, this, &SAKOtherAnalyzerSettingsWidget::bytesAnalysed);
    mAnalyzer->start();

    Q_ASSERT_X(settings, __FUNCTION__, "The parameter can not be nullptr!");
    if (settings){
        /// @brief 读入配置文件
        mFixedLengthCheckBox->setChecked(settings->value(mSettingKeyFixed).toBool());
        mLengthLineEdit->setText(settings->value(mSettingKeyLenth).toString());
        mStartLineEdit->setText(settings->value(mSettingKeyStartBytes).toString());
        mEndLineEdit->setText(settings->value(mSettingKeyEndBytes).toString());
        mDisableCheckBox->setChecked(!settings->value(mSettingKeyEnable).toBool());
    }
}

SAKOtherAnalyzerSettingsWidget::~SAKOtherAnalyzerSettingsWidget()
{
    delete mUi;
    delete mAnalyzer;
}

void SAKOtherAnalyzerSettingsWidget::inputBytes(QByteArray bytes)
{
    mAnalyzer->inputBytes(bytes);
}

void SAKOtherAnalyzerSettingsWidget::setLineEditFormat(QLineEdit *lineEdit)
{
    QRegExp regExpHex("([0-9A-F][0-9A-F][ ])*");
    if (lineEdit){
        lineEdit->setValidator(Q_NULLPTR);
        lineEdit->setValidator(new QRegExpValidator(regExpHex, this));
    }
}

void SAKOtherAnalyzerSettingsWidget::on_fixedLengthCheckBox_clicked()
{
    mAnalyzer->setFixed(mFixedLengthCheckBox->isChecked());
    if (mSettings){
        mSettings->setValue(mSettingKeyFixed, mFixedLengthCheckBox->isChecked());
    }
}

void SAKOtherAnalyzerSettingsWidget::on_lengthLineEdit_textChanged(const QString &text)
{
    mAnalyzer->setLength(text.toInt());
    if (mSettings){
        mSettings->setValue(mSettingKeyLenth, mLengthLineEdit->text());
    }
}

void SAKOtherAnalyzerSettingsWidget::on_startLineEdit_textChanged(const QString &text)
{
    text.trimmed();
    QStringList list = text.split(' ');
    QByteArray startBytes;
    for (auto var : list){
        quint8 v = var.toUInt(Q_NULLPTR, 16);
        startBytes.append(reinterpret_cast<char*>(&v), 1);
    }
    mAnalyzer->setStartArray(startBytes);
    if (mSettings){
        mSettings->setValue(mSettingKeyStartBytes, mStartLineEdit->text());
    }
}

void SAKOtherAnalyzerSettingsWidget::on_endLineEdit_textChanged(const QString &text)
{
    text.trimmed();
    QStringList list = text.split(' ');
    QByteArray endBytes;
    for (auto var : list){
        quint8 v = var.toUInt(Q_NULLPTR, 16);
        endBytes.append(reinterpret_cast<char*>(&v), 1);
    }
    mAnalyzer->setEndArray(endBytes);
    if (mSettings){
        mSettings->setValue(mSettingKeyEndBytes, mEndLineEdit->text());
    }
}

void SAKOtherAnalyzerSettingsWidget::on_disableCheckBox_clicked()
{
    mAnalyzer->setEnable(!mDisableCheckBox->isChecked());
    if (mSettings){
        mSettings->setValue(mSettingKeyEnable, !mDisableCheckBox->isChecked());
    }
}

void SAKOtherAnalyzerSettingsWidget::on_clearPushButton_clicked()
{
    mAnalyzer->clearData();
}
