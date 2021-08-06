/****************************************************************************************
 * Copyright 2021 Qter(qsaker@qq.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ***************************************************************************************/
#include <QMenu>
#include <QDebug>

#include "SAKDebuggerPlugins.hh"

SAKDebuggerPlugins::SAKDebuggerPlugins(QPushButton *readmeBt,
                                       QPushButton *menuBt,
                                       QSettings *settings,
                                       const QString &settingsGroup,
                                       QLabel *titleLabel,
                                       QSqlDatabase *sqlDatabase,
                                       QWidget *panelWidget,
                                       QObject *parent)
    :QObject(parent)
    ,mCharts(Q_NULLPTR)
    ,mDataForwarding(Q_NULLPTR)
    ,mRegularlySending(Q_NULLPTR)
    ,mAutomaticallyResponse(Q_NULLPTR)
    ,mTitleLabel(titleLabel)
    ,mPanelWidget(panelWidget)
{
    QMenu *menu = new QMenu(menuBt);
    menuBt->setMenu(menu);

    mTitleLabel->setHidden(true);
    mPanelWidget->setHidden(true);

    // Go to a web page.
    if (readmeBt) {
        connect(readmeBt, &QPushButton::clicked, this, [](){
            // Nothing to do yet.
        });
    }
    Q_UNUSED(settings);
    Q_UNUSED(settingsGroup);


    // Instance plugins.
    m3d = new SAKPlugin3d();
    mCharts = new SAKPluginCharts();
    mDataForwarding = new SAKPluginDataForwarding();
    mRegularlySending = new SAKDebuggerPluginRegularlySending(sqlDatabase,
                                                              settings,
                                                              settingsGroup,
                                                              "RegularlySending",
                                                              panelWidget);
    mAutomaticallyResponse = new SAKPluginAutomaticallyResponse(settings,
                                                                settingsGroup,
                                                                sqlDatabase);
    connect(this, &SAKDebuggerPlugins::bytesRead,
            mAutomaticallyResponse, &SAKPluginAutomaticallyResponse::onBytesRead);


    // Initialize menu psuh button
    QMenu *embedMenu = new QMenu(tr("Embed to Center"), menu);
    menu->addMenu(embedMenu);
    embedMenu->addAction(tr("Cancel Embedding"), this, [](){});
    embedMenu->addSeparator();
    struct SAKActionsContext {
        QString title;
        void (SAKDebuggerPlugins::*memberFunction)();
    };
    QVector<SAKActionsContext> actionsCtx;
    actionsCtx.append({tr("3D"), &SAKDebuggerPlugins::showPluin3D});
    actionsCtx.append({tr("Charts"), &SAKDebuggerPlugins::showPluinCharts});
    actionsCtx.append({tr("Data Forwarding"), &SAKDebuggerPlugins::showPluinDataForwarding});
    actionsCtx.append({tr("Regularly Sending"), &SAKDebuggerPlugins::showPluginRegularlySending});
    actionsCtx.append({tr("Automatically Response"), &SAKDebuggerPlugins::showPluginAutomaticallyResponse});
    auto addActionsToMenu = [=](QMenu *m, const QVector<SAKActionsContext> &ctxs){
        for (int i = 0; i < ctxs.count(); i++) {
            SAKActionsContext ctx = ctxs.at(i);
            m->addAction(ctx.title, this, ctx.memberFunction);
        }
    };
    addActionsToMenu(embedMenu, actionsCtx);
    menu->addSeparator();
    addActionsToMenu(menu, actionsCtx);
    menu->addSeparator();
    menu->addAction(tr("Automatically Reload"), this, [](){});
    menu->addAction(tr("Reload All"), this, [](){});
}

SAKDebuggerPlugins::~SAKDebuggerPlugins()
{
    m3d->deleteLater();
    mCharts->deleteLater();
    mDataForwarding->deleteLater();
    mRegularlySending->deleteLater();
    mAutomaticallyResponse->deleteLater();
}

void SAKDebuggerPlugins::showPluin3D()
{
    if (m3d->isHidden()) {
        m3d->show();
    } else {
        m3d->activateWindow();
    }
}

void SAKDebuggerPlugins::showPluinCharts()
{
    if (mCharts->isHidden()) {
        mCharts->show();
    } else {
        mCharts->activateWindow();
    }
}

void SAKDebuggerPlugins::showPluinDataForwarding()
{
    if (mDataForwarding->isHidden()) {
        mDataForwarding->show();
    } else {
        mDataForwarding->activateWindow();
    }
}

void SAKDebuggerPlugins::showPluginAutomaticallyResponse()
{
    if (mAutomaticallyResponse->isHidden()) {
        mAutomaticallyResponse->show();
    } else {
        mAutomaticallyResponse->activateWindow();
    }
}

void SAKDebuggerPlugins::showPluginRegularlySending()
{
    if (mRegularlySending->isHidden()) {
        mRegularlySending->show();
    } else {
        mRegularlySending->show();
    }
}
