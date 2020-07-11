/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoding with utf-8 (with BOM). It is a part of QtSwissArmyKnife
 * project(https://www.qsak.pro). The project is an open source project. You can
 * get the source of the project from: "https://github.com/qsak/QtSwissArmyKnife"
 * or "https://gitee.com/qsak/QtSwissArmyKnife". Also, you can join in the QQ
 * group which number is 952218522 to have a communication.
 */
#ifndef SAKCHARTSXYSERIALCHARTVIEW_HH
#define SAKCHARTSXYSERIALCHARTVIEW_HH

#include <QChartView>
#include <QRubberBand>

QT_CHARTS_USE_NAMESPACE

/// @brief 图表视图(对Qt官方示例的重构)
class SAKChartsXYSerialChartView : public QChartView
{
public:
    SAKChartsXYSerialChartView(QChart *chart, QWidget *parent = 0);
protected:
    bool viewportEvent(QEvent *event);
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
    void mouseDoubleClickEvent(QMouseEvent*event);
    void wheelEvent(QWheelEvent *event);
    void keyPressEvent(QKeyEvent *event);
private:
    bool mIsTouching;
};

#endif
