/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QTimer>
#include <QDialog>
#include <QAction>
#include <QHBoxLayout>
#include <QMapIterator>
#include <QRandomGenerator>

#include "SAKChartsXYSerialWidget.hh"
#include "SAKChartsXYSerialChartView.hh"
#include "SAKChartsXYSerialEditDialog.hh"
#include "SAKChartsXYSerialChartViewSettingsDialog.hh"

#include "ui_SAKChartsXYSerialWidget.h"

SAKChartsXYSerialWidget::SAKChartsXYSerialWidget(QWidget *parent)
    :QWidget (parent)
    ,mXYSerialEditDialog(new SAKChartsXYSerialEditDialog)
    ,mChartSettingsDialog (new SAKChartsXYSerialChartViewSettingsDialog)
    ,mUi (new Ui::SAKChartsXYSerialWidget)
{
    mUi->setupUi(this);
    mChartViewerWidget = mUi->chartViewerWidget;
    mChartSettingsPushButton = mUi->chartSettingsPushButton;
    mPauseCheckBox = mUi->pauseCheckBox;
    mClearPushButton = mUi->clearPushButton;
    mDeletePushButton = mUi->deletePushButton;
    mEditPushButton = mUi->editPushButton;
    mAddPushButton = mUi->addPushButton;

    /// @brief 初始化表格视图
    mChart = new QChart;
    mChartView = new SAKChartsXYSerialChartView(mChart, mChartViewerWidget);
    mChartView->setChart(mChart);
    mChartView->setRenderHint(QPainter::Antialiasing);

    mXAxis = new QDateTimeAxis;
    mXAxis->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
    mXAxis->setFormat(QString("mm:ss"));
    mXAxis->setTickCount(10);
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

    /// @brief 初始化映射变量，键为数据类型，值为成员函数指针
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeInt8, &SAKChartsXYSerialWidget::appendPointInt8);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeUint8, &SAKChartsXYSerialWidget::appendPointUint8);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeInt16, &SAKChartsXYSerialWidget::appendPointInt16);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeUint16, &SAKChartsXYSerialWidget::appendPointUint16);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeInt32, &SAKChartsXYSerialWidget::appendPointInt32);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeUint32, &SAKChartsXYSerialWidget::appendPointUint32);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeInt64, &SAKChartsXYSerialWidget::appendPointInt64);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeUint64, &SAKChartsXYSerialWidget::appendPointUint64);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeFloat32, &SAKChartsXYSerialWidget::appendPointFloat32);
    mAppendPointInterfaceMap.insert(SAKChartsXYSerialEditDialog::ParametersContext::ExtractParametersContext::DataTypeFloat64, &SAKChartsXYSerialWidget::appendPointFloat64);

#ifdef QT_DEBUG
    QTimer *testTimer = new QTimer(this);
    testTimer->setInterval(1000);
    connect(testTimer, &QTimer::timeout, this, [&](){
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
        quint64 value = QRandomGenerator::global()->generate()%100;
#else
        quint64 value = qrand()%100;
#endif
        QByteArray data(reinterpret_cast<char*>(&value), sizeof(8));
        inputBytes(data);
    });
    testTimer->start();
#endif
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
        SAKChartsXYSerialEditDialog::ParametersContext *ctx = reinterpret_cast<SAKChartsXYSerialEditDialog::ParametersContext *>(ptr);
        delete ctx;
    }
    mXYSerialParametersMap.clear();
}

void SAKChartsXYSerialWidget::inputBytes(QByteArray bytes)
{
    /// @brief 不处理数空数组,暂停输入时也不处理数据
    if (bytes.isEmpty() || mPauseCheckBox->isChecked()){
        return;
    }

    QMapIterator<QXYSeries *, void *> iterator(mXYSerialParametersMap);
    while (iterator.hasNext()) {
        iterator.next();
        QXYSeries *xySerial = iterator.key();
        void *ctx = iterator.value();
        /// @brief 添加坐标点
        appendPoint(xySerial, bytes, ctx);
    }
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
    SAKChartsXYSerialEditDialog::ParametersContext *ctx = reinterpret_cast<SAKChartsXYSerialEditDialog::ParametersContext *>(voidPtr);
    mXYSerialEditDialog->setParameters(*ctx);
    mXYSerialEditDialog->setWindowTitle(tr("编辑曲线图或散点图"));
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() == QDialog::Accepted){
        *ctx = mXYSerialEditDialog->parameters();
        xySerial->setColor(ctx->chartParameters.chartColor);
        xySerial->setName(ctx->chartParameters.chartName);
    }

    /// @brief 更改菜单项文本显示
    action->setText(ctx->chartParameters.chartName);
    for (auto var : mDeleteMenu->actions()){
        if (var->data().value<QXYSeries *>() == xySerial){
            var->setText(ctx->chartParameters.chartName);
            break;
        }
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

void SAKChartsXYSerialWidget::appendPoint(QXYSeries *xySerial, QByteArray frame, void *parametersCtx)
{
    SAKChartsXYSerialEditDialog::ParametersContext *ctx = reinterpret_cast<SAKChartsXYSerialEditDialog::ParametersContext *>(parametersCtx);
    int offset = ctx->extractParameters.startIndex-1;
    int dataType = ctx->extractParameters.dataType;
    int dataLength = mXYSerialEditDialog->lengthOfDataType(dataType);
    if (dataLength){
        int minLen = offset + dataLength;
        if (frame.length() >= minLen){
            QByteArray data(frame.data()+offset, dataLength);
            /// @brief 处理大端序
            if (ctx->extractParameters.isBigEndian){
                QByteArray temp;
                for (int i = 0; i < data.length(); i++){
                    temp.append(data.length()-1-i);
                }
                data = temp;
            }

            /// @brief 通过指针调用成员函数
            if (mAppendPointInterfaceMap.contains(ctx->extractParameters.dataType)){
                void (SAKChartsXYSerialWidget::*interface)(QByteArray, QXYSeries *) = mAppendPointInterfaceMap.value(dataType);
                (this->*interface)(data, xySerial);
            }
        }
    }
}

void SAKChartsXYSerialWidget::appendPointInt8(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<qint8>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointUint8(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<quint8>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointInt16(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<qint16>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointUint16(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<quint16>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointInt32(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<qint32>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointUint32(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<quint32>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointInt64(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<qint64>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointUint64(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<quint64>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointFloat32(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<float>(data, xySerial);
}

void SAKChartsXYSerialWidget::appendPointFloat64(QByteArray data, QXYSeries *xySerial)
{
    appendPointActually<double>(data, xySerial);
}

void SAKChartsXYSerialWidget::on_chartSettingsPushButton_clicked()
{
    mChartSettingsDialog->show();
}

void SAKChartsXYSerialWidget::on_clearPushButton_clicked()
{
    /// @brief 清空全部坐标点
    for (auto var : mChart->series()){
        reinterpret_cast<QXYSeries*>(var)->clear();
    }

    /// @brief 重置缩放，更新横坐标
    mChart->zoomReset();
    mXAxis->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
}

void SAKChartsXYSerialWidget::on_addPushButton_clicked()
{
    mXYSerialEditDialog->setWindowTitle(tr("添加曲线图或散点图"));
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() != QDialog::Accepted){
        return;
    }

    SAKChartsXYSerialEditDialog::ParametersContext ctx = mXYSerialEditDialog->parameters();
    QXYSeries *xySerial = Q_NULLPTR;
    if (ctx.chartParameters.chartType == SAKChartsXYSerialEditDialog::ParametersContext::ChartParametersContext::ChartTypeLine){
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
    SAKChartsXYSerialEditDialog::ParametersContext *ctxPtr = new SAKChartsXYSerialEditDialog::ParametersContext;
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
