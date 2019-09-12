/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * If you want to know more about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
 * Welcome to bother.
 *
 * I write the comment in English, it's not because that I'm good at English,
 * but for "installing B".
 */

#include "SAKDebugPage.hh"
#include "ReadWriteSettingsWidget.hh"
#include "ui_ReadWriteSettingsWidget.h"

ReadWriteSettingsWidget::ReadWriteSettingsWidget(SAKDebugPage *debugPage, QWidget* parent)
    :QDialog (parent)
    ,ui(new Ui::ReadWriteSettingsWidget)
    ,debugPage (debugPage)
{
    ui->setupUi(this);

    waitForReadyReadTime = 5;
    waitForBytesWrittenTime = 5;
}

void ReadWriteSettingsWidget::on_readParametersComboBox_currentTextChanged(const QString &text)
{
    waitForReadyReadTime = text.toInt();
    setReadWriteParameters();
}

void ReadWriteSettingsWidget::on_writeParametersComboBox_currentTextChanged(const QString &text)
{
    waitForBytesWrittenTime = text.toInt();
    setReadWriteParameters();
}

void ReadWriteSettingsWidget::setReadWriteParameters()
{
    struct SAKDebugPage::ReadWriteParameters parameters;
    parameters.waitForReadyReadTime = waitForReadyReadTime;
    parameters.waitForBytesWrittenTime = waitForBytesWrittenTime;

    debugPage->setReadWriteParameters(parameters);
}
