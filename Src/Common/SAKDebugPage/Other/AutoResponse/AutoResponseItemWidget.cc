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
#include "AutoResponseItemWidget.hh"
#include "ui_AutoResponseItemWidget.h"

#include <QDateTime>

AutoResponseItemWidget::AutoResponseItemWidget(SAKDebugPage *debugPage, QWidget *parent)
    :QWidget(parent)
    ,ui (new Ui::AutoResponseItemWidget)
    ,forbiddenAllAutoResponse (false)
    ,_debugPage (debugPage)
{
    ui->setupUi(this);
    remarkLineEdit              = ui->remarkLineEdit;
    referenceLineEdit           = ui->referenceLineEdit;
    responseLineEdit            = ui->responseLineEdit;
    enableCheckBox              = ui->enableCheckBox;
    optionComboBox              = ui->optionComboBox;
    referenceDataFromatComboBox = ui->referenceDataFromatComboBox;
    responseDataFormatComboBox  = ui->responseDataFormatComboBox;

    QStringList optionStrings;
    optionStrings << tr("接收数据与参考数据相同时自动回复")
                  << tr("接收数据包含参考数据时自动回复")
                  << tr("接收数据不包含参考数据时自动回复");
    optionComboBox->clear();
    optionComboBox->addItems(optionStrings);
}

AutoResponseItemWidget::~AutoResponseItemWidget()
{
    delete ui;
}

void AutoResponseItemWidget::setAllAutoResponseDisable(bool disAbel)
{
    forbiddenAllAutoResponse = disAbel;
}
