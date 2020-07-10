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
#include <QAction>
#include <QDateTime>
#include <QHBoxLayout>
#include <QMapIterator>

#include "SAKXYSerialEditDialog.hh"
#include "SAKChartSettingsDialog.hh"
#include "SAKChartsXYSerialWidget.hh"

#include "ui_SAKChartsXYSerialWidget.h"


SAKChartsXYSerialWidget::SAKChartsXYSerialWidget(QWidget *parent)
    :QWidget (parent)
    ,mXYSerialEditDialog(new SAKXYSerialEditDialog)
    ,mChartSettingsDialog (new SAKChartSettingsDialog)
    ,mUi (new Ui::SAKChartsXYSerialWidget)
{
    mUi->setupUi(this);
    mChartViewerWidget = mUi->chartViewerWidget;
    mChartSettingsPushButton = mUi->chartSettingsPushButton;
    mDeletePushButton = mUi->deletePushButton;
    mEditPushButton = mUi->editPushButton;
    mAddPushButton = mUi->addPushButton;

    /// @brief 初始化表格视图
    mChartView = new QChartView(mChartViewerWidget);
    mChart = new QChart;
    mChartView->setChart(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);

    mXAxis = new QDateTimeAxis;
    mXAxis->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
    mChart->addAxis(mXAxis, Qt::AlignBottom);

    mYAxis = new QValueAxis;
    mYAxis->setRange(0, 100);
    mChart->addAxis(mYAxis, Qt::AlignLeft);

    QHBoxLayout *layout = new QHBoxLayout(mChartViewerWidget);
    layout->addWidget(mChartView);
    mChartViewerWidget->setLayout(layout);

    /// @brief 菜单初始化
    mDeleteMenu = new QMenu(mDeletePushButton);
    mEditMenu = new QMenu(mEditPushButton);
    mDeletePushButton->setMenu(mDeleteMenu);
    mEditPushButton->setMenu(mEditMenu);
}

SAKChartsXYSerialWidget::~SAKChartsXYSerialWidget()
{
    delete mUi;
    delete mChartSettingsDialog;
    delete mXYSerialEditDialog;

    /// @brief 删除参数
    QMapIterator<QXYSeries *, void *> iterator(mXYSerialParametersMap);
    while (iterator.hasNext()) {
        iterator.next();
        void *ptr = iterator.value();
        SAKXYSerialEditDialog::ParametersContext *ctx = reinterpret_cast<SAKXYSerialEditDialog::ParametersContext *>(ptr);
        delete ctx;
    }
    mXYSerialParametersMap.clear();
}

void SAKChartsXYSerialWidget::inputBytes(QByteArray bytes)
{
    Q_UNUSED(bytes);
}

void SAKChartsXYSerialWidget::deleteXYSerial()
{
    /// @brief 确保是通过按钮信息号（删除按钮的菜单项触发）调用该函数的
    QAction *action = senderToAction(sender());
    if (!action){
         Q_ASSERT_X(false, __FUNCTION__, "Can not called the function directly");
        return;
    }

    /// @brief 删除相关资源
    QXYSeries *xySerial = action->data().value<QXYSeries *>();
    mChart->removeSeries(xySerial);
    mXYSerialParametersMap.remove(xySerial);
    action->deleteLater();
    for (auto var : mEditMenu->actions()){
        if (var->data().value<QXYSeries *>() == xySerial){
            var->deleteLater();
            break;
        }
    }
}

void SAKChartsXYSerialWidget::editXYSerial()
{
    /// @brief 确保是通过按钮信息号（删除按钮的菜单项触发）调用该函数的
    QAction *action = senderToAction(sender());
    if (!action){
         Q_ASSERT_X(false, __FUNCTION__, "Can not called the function directly");
        return;
    }

    QXYSeries *xySerial = action->data().value<QXYSeries *>();
    void *voidPtr = mXYSerialParametersMap.value(xySerial);
    SAKXYSerialEditDialog::ParametersContext *ctx = reinterpret_cast<SAKXYSerialEditDialog::ParametersContext *>(voidPtr);
    mXYSerialEditDialog->setParameters(*ctx);
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() == QDialog::Accepted){
        *ctx = mXYSerialEditDialog->parameters();
        xySerial->setColor(ctx->chartParameters.chartColor);
        xySerial->setName(ctx->chartParameters.chartName);
    }
}

QAction *SAKChartsXYSerialWidget::senderToAction(QObject *sender)
{
    QAction *action = Q_NULLPTR;
    if (sender){
        if (sender->inherits("QAction")){
            action = qobject_cast<QAction *>(sender);
        }
    }

    return action;
}

void SAKChartsXYSerialWidget::on_chartSettingsPushButton_clicked()
{
    mChartSettingsDialog->show();
}

void SAKChartsXYSerialWidget::on_addPushButton_clicked()
{
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() != QDialog::Accepted){
        return;
    }

    SAKXYSerialEditDialog::ParametersContext ctx = mXYSerialEditDialog->parameters();
    QXYSeries *xySerial = Q_NULLPTR;
    if (ctx.chartParameters.chartType == SAKXYSerialEditDialog::ParametersContext::ChartParametersContext::ChartTypeLine){
        xySerial = new QLineSeries;
    }else{
        xySerial = new QScatterSeries;
    }

    mChart->addSeries(xySerial);
    xySerial->append(QDateTime::currentMSecsSinceEpoch(), 1);
    xySerial->append(QDateTime::currentMSecsSinceEpoch()+1000, 1);
    xySerial->attachAxis(mXAxis);
    xySerial->attachAxis(mYAxis);
    xySerial->setColor(ctx.chartParameters.chartColor);
    xySerial->setName(ctx.chartParameters.chartName);

    /// @brief 保存参数
    SAKXYSerialEditDialog::ParametersContext *ctxPtr = new SAKXYSerialEditDialog::ParametersContext;
    *ctxPtr = ctx;
    mXYSerialParametersMap.insert(xySerial, ctxPtr);

    /// @brief 添加删除菜单选项
    QAction *action = new QAction(ctxPtr->chartParameters.chartName, this);
    action->setData(QVariant::fromValue(xySerial));
    mDeleteMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKChartsXYSerialWidget::deleteXYSerial);
    /// @brief 添加编辑菜单选项
    action = new QAction(ctxPtr->chartParameters.chartName, this);
    action->setData(QVariant::fromValue(xySerial));
    mEditMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKChartsXYSerialWidget::editXYSerial);
}
