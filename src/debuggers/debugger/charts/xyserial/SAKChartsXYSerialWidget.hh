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

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif

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
     * @brief inputBytes: Input data to the module
     * @param bytes: Data that input to the module
     */
    void inputBytes(QByteArray bytes);
private:
    SAKChartsXYSerialEditDialog *mXYSerialEditDialog;
    SAKChartsXYSerialChartViewSettingsDialog *mChartSettingsDialog;
    SAKChartsXYSerialChartView *mChartView;
    QChart *mChart;
    QValueAxis *mYAxis;
    QDateTimeAxis *mXAxis;

    QMap<QXYSeries *, void *> mXYSerialParametersMap;
    QMenu *mDeleteMenu;
    QMenu *mEditMenu;
    QMap<int, void (SAKChartsXYSerialWidget::*)(QByteArray, QXYSeries *, bool)> mAppendPointInterfaceMap;
private slots:
    void deleteXYSerial();
    void editXYSerial();
private:
    QAction *senderToAction(QObject *sender);

    void appendPoint(QXYSeries *xySerial, QByteArray frame, void *parametersCtx);
    void appendPointInt8(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointUint8(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointInt16(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointUint16(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointInt32(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointUint32(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointInt64(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointUint64(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointFloat32(QByteArray data, QXYSeries *xySerial, bool isBigEndian);
    void appendPointFloat64(QByteArray data, QXYSeries *xySerial, bool isBigEndian);

    template<typename T>
    void appendPointActually(QByteArray data, QXYSeries *xySerial, bool isBigEndian){
        if (data.length() < int(sizeof(T))){
            Q_ASSERT_X(false, __FUNCTION__, "Data error: lack of data!");
            return;
        }

        if (data.length() < int(sizeof(T))){
            return;
        }

        for (int i = 0; i < data.length()/int(sizeof(T)); i += sizeof(T)){
            T *ptr = reinterpret_cast<T *>(data.data() + i);
            qreal xValue = qreal(QDateTime::currentMSecsSinceEpoch());
            T yValue = *ptr;

            if (isBigEndian){
                T temp = *ptr;
                quint8 *tempTtr = reinterpret_cast<quint8*>(&temp);
                for (int i = 0; i < int(sizeof(T)); i++){
                    tempTtr[sizeof(T) - i] = ptr[i];
                }
                *ptr = temp;
            }

            xySerial->append(qreal(xValue), qreal(yValue));
            if (mXAxis->max().toMSecsSinceEpoch() < xValue){
                mChart->scroll(10, 0);
            }
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
