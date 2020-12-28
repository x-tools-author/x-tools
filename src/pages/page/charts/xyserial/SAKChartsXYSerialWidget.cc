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
#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
#include <QRandomGenerator>
#endif

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

    mDeleteMenu = new QMenu(mDeletePushButton);
    mEditMenu = new QMenu(mEditPushButton);
    mDeletePushButton->setMenu(mDeleteMenu);
    mEditPushButton->setMenu(mEditMenu);

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
    if (bytes.isEmpty() || mPauseCheckBox->isChecked()){
        return;
    }

    QMapIterator<QXYSeries *, void *> iterator(mXYSerialParametersMap);
    while (iterator.hasNext()) {
        iterator.next();
        QXYSeries *xySerial = iterator.key();
        void *ctx = iterator.value();

        appendPoint(xySerial, bytes, ctx);
    }
}

void SAKChartsXYSerialWidget::deleteXYSerial()
{
    QAction *action = senderToAction(sender());
    if (!action){
         Q_ASSERT_X(false, __FUNCTION__, "Can not called the function directly");
        return;
    }

    QXYSeries *xySerial = action->data().value<QXYSeries *>();
    mChart->removeSeries(xySerial);
    mXYSerialParametersMap.remove(xySerial);
    action->deleteLater();
    for (auto &var : mEditMenu->actions()){
        if (var->data().value<QXYSeries *>() == xySerial){
            var->deleteLater();
            break;
        }
    }
}

void SAKChartsXYSerialWidget::editXYSerial()
{
    QAction *action = senderToAction(sender());
    if (!action){
         Q_ASSERT_X(false, __FUNCTION__, "Can not called the function directly");
        return;
    }

    QXYSeries *xySerial = action->data().value<QXYSeries *>();
    void *voidPtr = mXYSerialParametersMap.value(xySerial);
    SAKChartsXYSerialEditDialog::ParametersContext *ctx = reinterpret_cast<SAKChartsXYSerialEditDialog::ParametersContext *>(voidPtr);
    mXYSerialEditDialog->setParameters(*ctx);
    mXYSerialEditDialog->setWindowTitle(tr("Edit line or scatter"));
    mXYSerialEditDialog->show();
    if (mXYSerialEditDialog->exec() == QDialog::Accepted){
        *ctx = mXYSerialEditDialog->parameters();
        xySerial->setColor(ctx->chartParameters.chartColor);
        xySerial->setName(ctx->chartParameters.chartName);
    }

    action->setText(ctx->chartParameters.chartName);
    for (auto &var : mDeleteMenu->actions()){
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
            if (ctx->extractParameters.isBigEndian){
                QByteArray temp;
                for (int i = 0; i < data.length(); i++){
                    temp.append(data.length()-1-i);
                }
                data = temp;
            }

            if (mAppendPointInterfaceMap.contains(ctx->extractParameters.dataType)){
                void (SAKChartsXYSerialWidget::*interface)(QByteArray, QXYSeries *, bool) = mAppendPointInterfaceMap.value(dataType);
                (this->*interface)(data, xySerial, ctx->extractParameters.isBigEndian);
            }
        }
    }
}

void SAKChartsXYSerialWidget::appendPointInt8(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<qint8>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointUint8(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<quint8>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointInt16(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<qint16>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointUint16(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<quint16>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointInt32(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<qint32>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointUint32(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<quint32>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointInt64(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<qint64>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointUint64(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<quint64>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointFloat32(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<float>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::appendPointFloat64(QByteArray data, QXYSeries *xySerial, bool isBigEndian)
{
    appendPointActually<double>(data, xySerial, isBigEndian);
}

void SAKChartsXYSerialWidget::on_chartSettingsPushButton_clicked()
{
    mChartSettingsDialog->show();
}

void SAKChartsXYSerialWidget::on_clearPushButton_clicked()
{
    for (auto &var : mChart->series()){
        reinterpret_cast<QXYSeries*>(var)->clear();
    }

    mChart->zoomReset();
    mXAxis->setRange(QDateTime::currentDateTime(), QDateTime::currentDateTime().addSecs(60));
}

void SAKChartsXYSerialWidget::on_addPushButton_clicked()
{
    mXYSerialEditDialog->setWindowTitle(tr("Add line or scatter"));
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

    SAKChartsXYSerialEditDialog::ParametersContext *ctxPtr = new SAKChartsXYSerialEditDialog::ParametersContext;
    *ctxPtr = ctx;
    mXYSerialParametersMap.insert(xySerial, ctxPtr);

    QAction *action = new QAction(ctxPtr->chartParameters.chartName, this);
    action->setData(QVariant::fromValue(xySerial));
    mDeleteMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKChartsXYSerialWidget::deleteXYSerial);

    action = new QAction(ctxPtr->chartParameters.chartName, this);
    action->setData(QVariant::fromValue(xySerial));
    mEditMenu->addAction(action);
    connect(action, &QAction::triggered, this, &SAKChartsXYSerialWidget::editXYSerial);
}
