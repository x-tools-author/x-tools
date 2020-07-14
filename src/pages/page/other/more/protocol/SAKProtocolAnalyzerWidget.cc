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

#include "SAKProtocolAnalyzer.hh"
#include "SAKProtocolAnalyzerWidget.hh"

#include "ui_SAKProtocolAnalyzerWidget.h"

SAKProtocolAnalyzerWidget::SAKProtocolAnalyzerWidget(QWidget *parent)
    :QWidget(parent)
    ,mAnalyzer(new SAKProtocolAnalyzer)
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
    connect(mAnalyzer, &SAKProtocolAnalyzer::bytesAnalized, this, &SAKProtocolAnalyzerWidget::bytesAnalysed);
    mAnalyzer->start();
}

SAKProtocolAnalyzerWidget::~SAKProtocolAnalyzerWidget()
{
    delete mUi;
    delete mAnalyzer;
}

void SAKProtocolAnalyzerWidget::inputBytes(QByteArray bytes)
{
    mAnalyzer->inputBytes(bytes);
}

void SAKProtocolAnalyzerWidget::setLineEditFormat(QLineEdit *lineEdit)
{
    QRegExp regExpHex("([0-9A-F][0-9A-F][ ])*");
    if (lineEdit){
        lineEdit->setValidator(Q_NULLPTR);
        lineEdit->setValidator(new QRegExpValidator(regExpHex, this));
    }
}

void SAKProtocolAnalyzerWidget::on_fixedLengthCheckBox_clicked()
{
    mAnalyzer->setFixed(mFixedLengthCheckBox->isChecked());
}

void SAKProtocolAnalyzerWidget::on_lengthLineEdit_textChanged(const QString &text)
{
    mAnalyzer->setLength(text.toInt());
}

void SAKProtocolAnalyzerWidget::on_startLineEdit_textChanged(const QString &text)
{
    text.trimmed();
    QStringList list = text.split(' ');
    QByteArray startBytes;
    for (auto var : list){
        quint8 v = var.toUInt(Q_NULLPTR, 16);
        startBytes.append(reinterpret_cast<char*>(&v), 1);
    }
    mAnalyzer->setStartArray(startBytes);
}

void SAKProtocolAnalyzerWidget::on_endLineEdit_textChanged(const QString &text)
{
    text.trimmed();
    QStringList list = text.split(' ');
    QByteArray endBytes;
    for (auto var : list){
        quint8 v = var.toUInt(Q_NULLPTR, 16);
        endBytes.append(reinterpret_cast<char*>(&v), 1);
    }
    mAnalyzer->setEndArray(endBytes);
}

void SAKProtocolAnalyzerWidget::on_disableCheckBox_clicked()
{
    mAnalyzer->setEnable(!mDisableCheckBox->isChecked());
}

void SAKProtocolAnalyzerWidget::on_clearPushButton_clicked()
{
    mAnalyzer->clearData();
}
