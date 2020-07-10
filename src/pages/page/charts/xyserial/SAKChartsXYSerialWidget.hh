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

#include <QMenu>
#include <QChart>
#include <QWidget>
#include <QDateTime>
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
class SAKChartSettingsDialog;
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
    SAKChartSettingsDialog *mChartSettingsDialog;
    QChartView *mChartView;
    QChart *mChart;
    QValueAxis *mYAxis;
    QDateTimeAxis *mXAxis;
    /// @brief 映射的键为曲线或者散点图的实例地址，值为曲线图或散点图的参数实例指针，数据类型为：SAKXYSerialEditDialog::ParametersContext
    QMap<QXYSeries *, void *> mXYSerialParametersMap;
    QMenu *mDeleteMenu;
    QMenu *mEditMenu;
    /// @brief 将数据类型映射为添加坐标点的接口
    QMap<int, void (SAKChartsXYSerialWidget::*)(QByteArray, QXYSeries *)> mAppendPointInterfaceMap;
private slots:
    void deleteXYSerial();
    void editXYSerial();
private:
    QAction *senderToAction(QObject *sender);
    /// @brief 添加坐标点, parametersCtx类型为SAKXYSerialEditDialog::ParametersContext实例指针
    void appendPoint(QXYSeries *xySerial, QByteArray frame, void *parametersCtx);
    void appendPointInt8(QByteArray data, QXYSeries *xySerial);
    void appendPointUint8(QByteArray data, QXYSeries *xySerial);
    void appendPointInt16(QByteArray data, QXYSeries *xySerial);
    void appendPointUint16(QByteArray data, QXYSeries *xySerial);
    void appendPointInt32(QByteArray data, QXYSeries *xySerial);
    void appendPointUin32(QByteArray data, QXYSeries *xySerial);
    void appendPointInt64(QByteArray data, QXYSeries *xySerial);
    void appendPointUint64(QByteArray data, QXYSeries *xySerial);
    void appendPointFloat16(QByteArray data, QXYSeries *xySerial);
    void appendPointFloat32(QByteArray data, QXYSeries *xySerial);
    void appendPointFloat64(QByteArray data, QXYSeries *xySerial);
    /// @brief 添加坐标点函数模板
    template<typename T>
    void appendPointActually(QByteArray data, QXYSeries *xySerial){
        if (data.length() < sizeof(T)){
            Q_ASSERT_X(false, __FUNCTION__, "Data error: lack of data!");
            return;
        }

        T *ptr = reinterpret_cast<T *>(data.data());
        T yValue = *ptr;
        xySerial->append(qreal(QDateTime::currentMSecsSinceEpoch()), qreal(yValue));
    }
private:
    Ui::SAKChartsXYSerialWidget *mUi;
    QWidget *mChartViewerWidget;
    QPushButton *mChartSettingsPushButton;
    QPushButton *mDeletePushButton;
    QPushButton *mEditPushButton;
    QPushButton *mAddPushButton;
private slots:
    void on_chartSettingsPushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
