/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#ifndef SAKCHARTSXYSERIALWIDGET_HH
#define SAKCHARTSXYSERIALWIDGET_HH

#include <QMenu>
#include <QWidget>
#include <QDateTime>
#include <QCheckBox>
#include <QTabWidget>
#include <QPushButton>
#include <QSqlDatabase>

#include "qcustomplot/qcustomplot.h"

namespace Ui {
    class SAKChartsXYSerial;
};


class SAKChartsXYSerial : public QCustomPlot
{
    Q_OBJECT
public:
    SAKChartsXYSerial(QSqlDatabase *sqlDatabase,
                      QSettings *settings,
                      QString settingsGroup,
                      QString tableNameSuffix,
                      QWidget *parent = Q_NULLPTR);
    ~SAKChartsXYSerial();
    void inputAFrame(QString frame);


protected:
    void contextMenuEvent(QContextMenuEvent* e) final;


private:
    enum SAKEnumXYGraphType {
        XYGraphTypeLine,
        XYGraphTypeSpline,
        XYGraphTypeScator
    };


private:
    QSqlDatabase *mSqlDatabase;
    QSettings *mSettings;
    QString mSettingsGroup;
    QString mTableNameSuffix;

    const int mMaxChannelsNumber = 16;
    QMap<QString, int> mXYGraphTypeMap;
    QMap<QPair<QString, int>, QCPGraph*> mGraphsMap;
};

#endif
