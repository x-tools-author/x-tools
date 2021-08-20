/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QTimer>
#include <QDialog>
#include <QAction>
#include <QHBoxLayout>
#include <QMapIterator>

#include "SAKChartsXYSerial.hh"
#include "ui_SAKChartsXYSerial.h"

SAKChartsXYSerial::SAKChartsXYSerial(QWidget *parent)
    :QCustomPlot(parent)
{
    mXYGraphTypeMap.insert("Line", XYGraphTypeLine);
    mXYGraphTypeMap.insert("Spline", XYGraphTypeSpline);
    mXYGraphTypeMap.insert("Scatter", XYGraphTypeScator);

    QMapIterator<QString, int> iter(mXYGraphTypeMap);
    while (iter.hasNext()) {
        iter.next();
        const QString graphName = iter.key();
        for (int i = 0; i < mMaxChannelsNumber; i++) {
            QCPGraph *graph = addGraph();
            QPair<QString, int> nameIndex(graphName.toUpper(), i);
            mGraphsMap.insert(nameIndex, graph);
        }
    }

    // Allow user to drag axis ranges with mouse,
    // zoom with mouse wheel and select graphs by clicking.
    setInteractions(QCP::iRangeDrag | QCP::iRangeZoom
                    | QCP::iSelectPlottables | QCP::iSelectPlottablesBeyondAxisRect);
}

SAKChartsXYSerial::~SAKChartsXYSerial()
{

}

void SAKChartsXYSerial::inputAFrame(QString frame)
{
    frame = frame.remove(QRegularExpression("[\r\n]."));
    QStringList frameSections = frame.split(":", QString::SkipEmptyParts);
    QString dataFlag = frameSections.takeFirst().toUpper();
    QStringList dataList = frameSections.last().split(",", QString::SkipEmptyParts);

    QMapIterator<QString, int> iter(mXYGraphTypeMap);
    bool hasDataFlag = false;
    while (iter.hasNext()) {
        iter.next();
        const QString graphName = iter.key();
        if (dataFlag.toUpper() == graphName.toUpper()) {
            hasDataFlag = true;
            break;
        }
    }

    if (hasDataFlag) {
        for (int i = 0; i < dataList.count(); i++) {
            QPair<QString, int> nameIndex(dataFlag.toUpper(), i);
            if (mGraphsMap.contains(nameIndex)) {
                QCPGraph *graph = mGraphsMap.value(nameIndex);
                QVector<double> keys;
                keys << QDateTime::currentMSecsSinceEpoch();
                QVector<double> values;
                values << dataList.at(i).toDouble();
                graph->addData(keys, values);
            }
        }
    }
}

void SAKChartsXYSerial::contextMenuEvent(QContextMenuEvent* e)
{
    QMenu *menu = new QMenu();
    menu->addAction(tr("Reset Graph"));
    menu->addAction(tr("Import Graph Data"));
    menu->addAction(tr("Export Graph Data"));
    menu->addSeparator();

    QMenu *visibleMenu = new QMenu(tr("Graph Visible"), menu);
    menu->addMenu(visibleMenu);
    QMapIterator<QString, int> iter(mXYGraphTypeMap);
    while (iter.hasNext()) {
        iter.next();
        const QString graphName = iter.key();
        QMenu *graphMenu = new QMenu(graphName, visibleMenu);
        visibleMenu->addMenu(graphMenu);
        for (int i = 0; i < mMaxChannelsNumber; i++) {
            graphMenu->addAction(graphName + " " + QString::number(i+1));
        }
    }

    menu->exec(e->globalPos());
    delete menu;
}
