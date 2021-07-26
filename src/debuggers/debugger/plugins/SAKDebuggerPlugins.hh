/*
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 */
#ifndef SAKDEBUGGERPLUGINS_HH
#define SAKDEBUGGERPLUGINS_HH

#include <QLabel>
#include <QWidget>
#include <QObject>
#include <QSettings>
#include <QPushButton>
#include <QSqlDatabase>

#include "SAKPlugin3d.hh"
#include "SAKPluginCharts.hh"
#include "SAKPluginDataForwarding.hh"
#include "SAKPluginRegularlySending.hh"
#include "SAKPluginAutomaticallyResponse.hh"

class SAKDebuggerPlugins : public QObject
{
    Q_OBJECT
public:
    explicit SAKDebuggerPlugins(QPushButton *readmeBt,
                                QPushButton *menuBt,
                                QSettings *settings,
                                const QString &settingsGroup,
                                QLabel *titleLabel,
                                QSqlDatabase *sqlDatabase,
                                QWidget *panelWidget,
                                QObject *parent = Q_NULLPTR);
    ~SAKDebuggerPlugins();
private:
    SAKPlugin3d *m3d;
    SAKPluginCharts *mCharts;
    SAKPluginDataForwarding *mDataForwarding;
    SAKPluginRegularlySending *mRegularlySending;
    SAKPluginAutomaticallyResponse *mAutomaticallyResponse;
private:
    QLabel *mTitleLabel;
    QWidget *mPanelWidget;
private:
    void showPluin3D();
    void showPluinCharts();
    void showPluinDataForwarding();
    void showPluginRegularlySending();
    void showPluginAutomaticallyResponse();
};

#endif // SAKDEBUGGERPLUGINS_HH
