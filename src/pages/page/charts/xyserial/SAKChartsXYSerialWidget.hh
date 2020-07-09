/*
 * Copyright 2020 Qter(qsak@foxmail.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKCHARTSXYSERIALWIDGET_HH
#define SAKCHARTSXYSERIALWIDGET_HH

#include <QChart>
#include <QWidget>
#include <QValueAxis>
#include <QTabWidget>
#include <QPushButton>
#include <QLineSeries>
#include <QDateTimeAxis>
#include <QScatterSeries>

#include <QChartView>
QT_CHARTS_USE_NAMESPACE

namespace Ui {
    class SAKChartsXYSerialWidget;
};

class SAKXYSerialEditDialog;
/// @brief 散点图及曲线图显示类
class SAKChartsXYSerialWidget:public QWidget
{
    Q_OBJECT
public:
    SAKChartsXYSerialWidget(QWidget *parent = Q_NULLPTR);
    ~SAKChartsXYSerialWidget();

    /**
     * @brief inputBytes 数据输入接口，参数bytes按照一帧数据来处理
     * @param bytes 数据
     */
    void inputBytes(QByteArray bytes);
private:
    SAKXYSerialEditDialog *mXYSerialEditDialog;
    QChartView *mChartView;
    QChart *mChart;
    QValueAxis *mYAxis;
    QDateTimeAxis *mXAxis;
private:
    Ui::SAKChartsXYSerialWidget *mUi;
    QWidget *mChartViewerWidget;
    QPushButton *mChartSettingsPushButton;
    QPushButton *mDeletePushButton;
    QPushButton *mEditPushButton;
    QPushButton *mAddPushButton;
private slots:
    void on_chartSettingsPushButton_clicked();
    void on_editPushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
