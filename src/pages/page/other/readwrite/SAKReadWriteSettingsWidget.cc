/*
 * Copyright 2018-2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include "SAKDebugPage.hh"
#include "SAKReadWriteSettingsWidget.hh"
#include "ui_SAKReadWriteSettingsWidget.h"

SAKReadWriteSettingsWidget::SAKReadWriteSettingsWidget(SAKDebugPage *debugPage, QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::SAKReadWriteSettingsWidget)
    ,debugPage (debugPage)
{
    ui->setupUi(this);

    waitForReadyReadTime = 5;
    waitForBytesWrittenTime = 5;
}

void SAKReadWriteSettingsWidget::on_readParametersComboBox_currentTextChanged(const QString &text)
{
    waitForReadyReadTime = text.toInt();
    setReadWriteParameters();
}

void SAKReadWriteSettingsWidget::on_writeParametersComboBox_currentTextChanged(const QString &text)
{
    waitForBytesWrittenTime = text.toInt();
    setReadWriteParameters();
}

void SAKReadWriteSettingsWidget::on_intervalParameterComboBox_currentTextChanged(const QString &text)
{
    runIntervalTime = text.toInt();
    setReadWriteParameters();
}

void SAKReadWriteSettingsWidget::setReadWriteParameters()
{
    struct SAKDebugPage::ReadWriteParameters parameters;
    parameters.waitForReadyReadTime = waitForReadyReadTime;
    parameters.waitForBytesWrittenTime = waitForBytesWrittenTime;
    parameters.runIntervalTime = runIntervalTime;

    debugPage->setReadWriteParameters(parameters);
}
