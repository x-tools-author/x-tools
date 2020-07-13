/*
 * Copyright 2018-2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
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
}

SAKProtocolAnalyzerWidget::~SAKProtocolAnalyzerWidget()
{
    delete mUi;
    delete mAnalyzer;
}

void SAKProtocolAnalyzerWidget::on_fixedLengthCheckBox_clicked()
{

}

void SAKProtocolAnalyzerWidget::on_lengthLineEdit_textChanged(const QString &text)
{

}

void SAKProtocolAnalyzerWidget::on_startLineEdit_textChanged(const QString &text)
{

}

void SAKProtocolAnalyzerWidget::on_endLineEdit_textChanged(const QString &text)
{

}

void SAKProtocolAnalyzerWidget::on_disableCheckBox_clicked()
{

}
