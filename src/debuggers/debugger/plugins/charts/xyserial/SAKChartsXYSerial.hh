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
#include <QWidget>
#include <QDateTime>
#include <QCheckBox>
#include <QTabWidget>
#include <QPushButton>

#include "qcustomplot/qcustomplot.h"

namespace Ui {
    class SAKChartsXYSerial;
};

class SAKChartsXYSerial : public QCustomPlot
{
    Q_OBJECT
public:
    SAKChartsXYSerial(QWidget *parent = Q_NULLPTR);
    ~SAKChartsXYSerial();


private:
    enum SAKEnumXYSerialType {
        XYSerialTypeLine,
        XYSerialTypeSpline,
        XYSerialTypeScator
    };


private:
    QMap<int, QString> mXYSerialTypeNamesMap;
};

#endif
