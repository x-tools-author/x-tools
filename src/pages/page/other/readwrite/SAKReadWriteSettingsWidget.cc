/*
 * Copyright (C) 2018-2019 wuuhii. All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project. The project is a open source project, you can get the source from:
 *     https://github.com/wuuhii/QtSwissArmyKnife
 *     https://gitee.com/wuuhii/QtSwissArmyKnife
 *
 * For more information about the project, please join our QQ group(952218522).
 * In addition, the email address of the project author is wuuhii@outlook.com.
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

void SAKReadWriteSettingsWidget::setReadWriteParameters()
{
    struct SAKDebugPage::ReadWriteParameters parameters;
    parameters.waitForReadyReadTime = waitForReadyReadTime;
    parameters.waitForBytesWrittenTime = waitForBytesWrittenTime;

    debugPage->setReadWriteParameters(parameters);
}
