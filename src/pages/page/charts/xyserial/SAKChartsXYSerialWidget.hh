/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCHARTSXYSERIALWIDGET_HH
#define SAKCHARTSXYSERIALWIDGET_HH

#include <QMenu>
#include <QChart>
#include <QWidget>
#include <QDateTime>
#include <QCheckBox>
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

class SAKChartsXYSerialChartView;
class SAKChartsXYSerialEditDialog;
class SAKChartsXYSerialChartViewSettingsDialog;
// Charts
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
    SAKChartsXYSerialEditDialog *mXYSerialEditDialog;
    SAKChartsXYSerialChartViewSettingsDialog *mChartSettingsDialog;
    SAKChartsXYSerialChartView *mChartView;
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
    void appendPointUint32(QByteArray data, QXYSeries *xySerial);
    void appendPointInt64(QByteArray data, QXYSeries *xySerial);
    void appendPointUint64(QByteArray data, QXYSeries *xySerial);
    void appendPointFloat32(QByteArray data, QXYSeries *xySerial);
    void appendPointFloat64(QByteArray data, QXYSeries *xySerial);
    /// @brief 添加坐标点函数模板
    template<typename T>
    void appendPointActually(QByteArray data, QXYSeries *xySerial){
        if (data.length() < sizeof(T)){
            Q_ASSERT_X(false, __FUNCTION__, "Data error: lack of data!");
            return;
        }

        /// @brief 添加坐标点
        T *ptr = reinterpret_cast<T *>(data.data());
        qreal xValue = qreal(QDateTime::currentMSecsSinceEpoch());
        T yValue = *ptr;
        xySerial->append(xValue, qreal(yValue));

        /// @brief 移动图表，使得新显示在可见区域
        if (mXAxis->max().toMSecsSinceEpoch() < xValue){
            mChart->scroll(10, 0);
        }
    }
private:
    Ui::SAKChartsXYSerialWidget *mUi;
    QWidget *mChartViewerWidget;
    QPushButton *mChartSettingsPushButton;
    QCheckBox *mPauseCheckBox;
    QPushButton *mClearPushButton;
    QPushButton *mDeletePushButton;
    QPushButton *mEditPushButton;
    QPushButton *mAddPushButton;
private slots:
    void on_chartSettingsPushButton_clicked();
    void on_clearPushButton_clicked();
    void on_addPushButton_clicked();
};

#endif
