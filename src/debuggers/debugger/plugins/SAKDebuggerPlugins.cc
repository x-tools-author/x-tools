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

#include "SAKMainWindow.hh"
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
    ,mAutoResponse(Q_NULLPTR)
    ,mTitleLabel(titleLabel)
    ,mPanelWidget(panelWidget)
    ,mPluginDialog(Q_NULLPTR)
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
                                                              "RegularlySending");
    mAutoResponse = new SAKDebuggerPluginAutoResponse(settings,
                                                      settingsGroup,
                                                      sqlDatabase,
                                                      "AutoResponse");
    connect(this, &SAKDebuggerPlugins::bytesRead,
            mAutoResponse, &SAKDebuggerPluginAutoResponse::onBytesRead);


    // Initialize menu psuh button
    QMenu *embedMenu = new QMenu(tr("Embed to Center"), menu);
    menu->addMenu(embedMenu);
    embedMenu->addAction(tr("Cancel Embedding"),
                         this,
                         &SAKDebuggerPlugins::cancelEmbedPlugin);
    embedMenu->addSeparator();
    struct SAKActionsContext {
        QString title;
        void (SAKDebuggerPlugins::*show)();
        void (SAKDebuggerPlugins::*embed)();
    };
    QVector<SAKActionsContext> actionsCtx;
    actionsCtx.append({tr("3D"),
                       &SAKDebuggerPlugins::showPluin3D,
                       &SAKDebuggerPlugins::embedPluin3D});
    actionsCtx.append({tr("Charts"),
                       &SAKDebuggerPlugins::showPluinCharts,
                       &SAKDebuggerPlugins::embedPluinCharts});
    actionsCtx.append({tr("Data Forwarding"),
                       &SAKDebuggerPlugins::showPluinDataForwarding,
                       &SAKDebuggerPlugins::embedPluinDataForwarding});
    actionsCtx.append({tr("Regularly Sending"),
                       &SAKDebuggerPlugins::showPluginRegularlySending,
                       &SAKDebuggerPlugins::embedPluginRegularlySending});
    actionsCtx.append({tr("Automatically Response"),
                       &SAKDebuggerPlugins::showPluginAutoResponse,
                       &SAKDebuggerPlugins::embedPluginAutoResponse});
    auto addActionsToMenu = [=](QMenu *m,
            const QVector<SAKActionsContext> &ctxs,
            bool isShow){
        for (int i = 0; i < ctxs.count(); i++) {
            SAKActionsContext ctx = ctxs.at(i);
            if (isShow) {
                m->addAction(ctx.title, this, ctx.show);
            } else {
                m->addAction(ctx.title, this, ctx.embed);
            }
        }
    };
    addActionsToMenu(embedMenu, actionsCtx, false);
    menu->addSeparator();
    addActionsToMenu(menu, actionsCtx, true);
    menu->addSeparator();
    menu->addAction(tr("Automatically Reload"), this, [](){});
    menu->addAction(tr("Reload All"), this, [](){});

    mPluginDialog = new QDialog(sakMainWindow);
    mPluginDialog->setLayout(new QVBoxLayout(mPluginDialog));
    mPluginDialog->setModal(true);
    mPluginDialog->layout()->setContentsMargins(0, 0, 0, 0);
    mPluginDialog->setContentsMargins(6, 6, 6, 6);
    mPluginDialog->resize(800, 350);


    if (mPanelWidget) {
        if (!mPanelWidget->layout()) {
            mPanelWidget->setLayout(new QVBoxLayout(mPanelWidget));
            mPanelWidget->layout()->setContentsMargins(0, 0, 0, 0);
            mPanelWidget->setContentsMargins(0, 0, 0, 0);
        }
    } else {
        Q_ASSERT_X(mPanelWidget, __FUNCTION__, "Can not be null(mPanelWidget)!");
        mPanelWidget = new QWidget(sakMainWindow);
        mPanelWidget->hide();
    }
}

SAKDebuggerPlugins::~SAKDebuggerPlugins()
{
    m3d->deleteLater();
    mCharts->deleteLater();
    mDataForwarding->deleteLater();
    mRegularlySending->deleteLater();
    mAutoResponse->deleteLater();
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

void SAKDebuggerPlugins::showPluginAutoResponse()
{
    showPluginDialog(mAutoResponse);
}

void SAKDebuggerPlugins::showPluinDataForwarding()
{
    if (mDataForwarding->isHidden()) {
        mDataForwarding->show();
    } else {
        mDataForwarding->activateWindow();
    }
}

void SAKDebuggerPlugins::showPluginRegularlySending()
{
    showPluginDialog(mRegularlySending);
}

void SAKDebuggerPlugins::showPluginDialog(QWidget *contentWidget)
{
    for (int i = 0; i < mPluginDialog->layout()->count(); i++) {
        auto ret = mPluginDialog->layout()->takeAt(i);
        ret->widget()->setParent(Q_NULLPTR);
    }

    mPluginDialog->layout()->addWidget(contentWidget);
    mPluginDialog->show();
    cancelEmbedPlugin();
}

void SAKDebuggerPlugins::embedPluin3D()
{
    embedPlugin(m3d);
}

void SAKDebuggerPlugins::embedPluinCharts()
{
    embedPlugin(mCharts);
}

void SAKDebuggerPlugins::embedPluginAutoResponse()
{
    embedPlugin(mAutoResponse);
}

void SAKDebuggerPlugins::embedPluinDataForwarding()
{
    embedPlugin(mDataForwarding);
}

void SAKDebuggerPlugins::embedPluginRegularlySending()
{
    embedPlugin(mRegularlySending);
}

void SAKDebuggerPlugins::embedPlugin(QWidget *contentWidget)
{
    for (int i = 0; i < mPanelWidget->layout()->count(); i++) {
        auto ret = mPanelWidget->layout()->takeAt(i);
        ret->widget()->setParent(Q_NULLPTR);
    }

    mPanelWidget->layout()->addWidget(contentWidget);
    mTitleLabel->show();
    mPanelWidget->show();
}

void SAKDebuggerPlugins::cancelEmbedPlugin()
{
    for (int i = 0; i < mPanelWidget->layout()->count(); i++) {
        auto ret = mPanelWidget->layout()->takeAt(i);
        ret->widget()->setParent(Q_NULLPTR);
    }

    mTitleLabel->hide();
    mPanelWidget->hide();
}
