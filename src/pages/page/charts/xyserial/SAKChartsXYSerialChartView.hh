/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKCHARTSXYSERIALCHARTVIEW_HH
#define SAKCHARTSXYSERIALCHARTVIEW_HH

#include <QChartView>
#include <QRubberBand>

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
QT_CHARTS_USE_NAMESPACE
#endif

/// @brief Chart viewe
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
