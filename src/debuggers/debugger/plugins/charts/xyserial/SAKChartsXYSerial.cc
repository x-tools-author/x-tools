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

SAKChartsXYSerial::SAKChartsXYSerial(QSqlDatabase *sqlDatabase,
                                     QSettings *settings,
                                     QString settingsGroup,
                                     QString tableNameSuffix,
                                     QWidget *parent)
    :QCustomPlot(parent)
    ,mSqlDatabase(sqlDatabase)
    ,mSettings(settings)
    ,mSettingsGroup(settingsGroup)
    ,mTableNameSuffix(tableNameSuffix)
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

#if 0
    QVector<double> keys;
    QVector<double> values;
    for (int i = 0; i < 1000; i++) {
        keys << i;
        values << i*i;
    }
    auto graph = mGraphsMap.value(
                QPair<QString, int>(mXYGraphTypeMap.key(XYGraphTypeSpline).toUpper(),
                                   XYGraphTypeSpline));
    graph->addData(keys, values);
#endif
}

SAKChartsXYSerial::~SAKChartsXYSerial()
{

}

void SAKChartsXYSerial::onBytesRead(QByteArray bytes)
{
    QString frames = QString::fromLatin1(bytes);

    frames = frames.remove(QRegularExpression("[\r\n]."));
    QStringList frameSections = frames.split(":", QString::SkipEmptyParts);
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
                double key = i; // QDateTime::currentMSecsSinceEpoch();
                double value = dataList.at(i).toDouble();
                graph->addData(key, value);
            }
        }
    }
}

void SAKChartsXYSerial::contextMenuEvent(QContextMenuEvent* e)
{
    QMenu *menu = new QMenu();
    menu->addAction(tr("Reset graph"));
    menu->addAction(tr("Import graph data"));
    menu->addAction(tr("Export graph data"));
    menu->addSeparator();

    QMenu *visibleMenu = new QMenu(tr("Graph visible"), menu);
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
