/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#include <QDialog>
#include <QHBoxLayout>

#include "SAKXYSerialEditDialog.hh"
#include "SAKChartsXYSerialWidget.hh"

#include "ui_SAKChartsXYSerialWidget.h"


SAKChartsXYSerialWidget::SAKChartsXYSerialWidget(QWidget *parent)
    :QWidget (parent)
    ,mXYSerialEditDialog(new SAKXYSerialEditDialog)
    ,mUi (new Ui::SAKChartsXYSerialWidget)
{
    mUi->setupUi(this);
    mChartViewerWidget = mUi->chartViewerWidget;
    mDeletePushButton = mUi->deletePushButton;
    mEditPushButton = mUi->editPushButton;
    mAddPushButton = mUi->addPushButton;

    /// @brief 初始化表格视图
    mChartView = new QChartView(mChartViewerWidget);
    QHBoxLayout *layout = new QHBoxLayout(mChartViewerWidget);
    layout->addWidget(mChartView);
    mChartViewerWidget->setLayout(layout);
}

SAKChartsXYSerialWidget::~SAKChartsXYSerialWidget()
{
    delete mUi;
}

void SAKChartsXYSerialWidget::inputBytes(QByteArray bytes)
{
    Q_UNUSED(bytes);
}

void SAKChartsXYSerialWidget::on_editPushButton_clicked()
{
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() != QDialog::Accepted){
        return;
    }
}

void SAKChartsXYSerialWidget::on_addPushButton_clicked()
{
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() != QDialog::Accepted){
        return;
    }
}
