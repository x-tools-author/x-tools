/*
 * Copyright 2020 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#include <QMouseEvent>
#include <QWheelEvent>

#include "SAKChartsXYSerialChartView.hh"

SAKChartsXYSerialChartView::SAKChartsXYSerialChartView(QChart *chart, QWidget *parent) :
    QChartView(chart, parent),
    mIsTouching(false)
{
    setRubberBand(QChartView::RectangleRubberBand);
}

bool SAKChartsXYSerialChartView::viewportEvent(QEvent *event)
{
    if (event->type() == QEvent::TouchBegin) {
        // By default touch events are converted to mouse events. So
        // after this event we will get a mouse event also but we want
        // to handle touch events as gestures only. So we need this safeguard
        // to block mouse events that are actually generated from touch.
        mIsTouching = true;

        // Turn off animations when handling gestures they
        // will only slow us down.
        chart()->setAnimationOptions(QChart::NoAnimation);
    }
    return QChartView::viewportEvent(event);
}

void SAKChartsXYSerialChartView::mousePressEvent(QMouseEvent *event)
{
    if (mIsTouching){
        return;
    }

    QChartView::mousePressEvent(event);
}

void SAKChartsXYSerialChartView::mouseMoveEvent(QMouseEvent *event)
{
    if (mIsTouching){
        return;
    }

    QChartView::mouseMoveEvent(event);
}

void SAKChartsXYSerialChartView::mouseReleaseEvent(QMouseEvent *event)
{
    if (mIsTouching){
        mIsTouching = false;
    }

    // Because we disabled animations when touch event was detected
    // we must put them back on.
    chart()->setAnimationOptions(QChart::SeriesAnimations);

    QChartView::mouseReleaseEvent(event);
}

void SAKChartsXYSerialChartView::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (mIsTouching){
        mIsTouching = false;
    }

    chart()->zoomReset();
    QChartView::mouseDoubleClickEvent(event);
}

void SAKChartsXYSerialChartView::wheelEvent(QWheelEvent *event)
{
    if (mIsTouching){
        mIsTouching = false;
    }

    if (event->angleDelta().y() > 0){
        chart()->zoomOut();
    }else{
        chart()->zoomIn();
    }

    QChartView::wheelEvent(event);
}

void SAKChartsXYSerialChartView::keyPressEvent(QKeyEvent *event)
{
    switch (event->key()) {
    case Qt::Key_Plus:
        chart()->zoomIn();
        break;
    case Qt::Key_Minus:
        chart()->zoomOut();
        break;
    case Qt::Key_Left:
        chart()->scroll(-10, 0);
        break;
    case Qt::Key_Right:
        chart()->scroll(10, 0);
        break;
    case Qt::Key_Up:
        chart()->scroll(0, 10);
        break;
    case Qt::Key_Down:
        chart()->scroll(0, -10);
        break;
    default:
        QGraphicsView::keyPressEvent(event);
        break;
    }
}
