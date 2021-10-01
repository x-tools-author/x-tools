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
    ,m3d(Q_NULLPTR)
    //,mCharts(Q_NULLPTR)
    ,mTransponders(Q_NULLPTR)
    ,mAutoResponse(Q_NULLPTR)
    ,mTimedSending(Q_NULLPTR)
    ,mTitleLabel(titleLabel)
    ,mPanelWidget(panelWidget)
    ,mPluginDialog(Q_NULLPTR)
    ,mActiveWidgetInPanel(Q_NULLPTR)
    ,mActiveWidgetInDialog(Q_NULLPTR)
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
#if 0
    mCharts = new SAKPluginCharts(sqlDatabase,
                                  settings,
                                  settingsGroup,
                                  "Charts");
#endif
    mTransponders = new SAKDebuggerPluginTransponders(sqlDatabase,
                                                      settings,
                                                      settingsGroup,
                                                      "Transponders");
    connect(this, &SAKDebuggerPlugins::bytesRead,
            mTransponders, &SAKDebuggerPluginTransponders::bytesRead);
    connect(mTransponders, &SAKDebuggerPluginTransponders::invokeWriteCookedBytes,
            this, &SAKDebuggerPlugins::invokeWriteCookedBytes);
    mTimedSending = new SAKDebuggerPluginTimedSending(sqlDatabase,
                                                      settings,
                                                      settingsGroup,
                                                      "TimedSending");
    connect(mTimedSending, &SAKDebuggerPluginTimedSending::invokeWriteCookedBytes,
            this, &SAKDebuggerPlugins::invokeWriteCookedBytes);
    mAutoResponse = new SAKDebuggerPluginAutoResponse(settings,
                                                      settingsGroup,
                                                      sqlDatabase,
                                                      "AutoResponse");
    connect(this, &SAKDebuggerPlugins::bytesRead,
            mAutoResponse, &SAKDebuggerPluginAutoResponse::onBytesRead);
    connect(mAutoResponse, &SAKDebuggerPluginAutoResponse::invokeWriteCookedBytes,
            this, &SAKDebuggerPlugins::invokeWriteCookedBytes);


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
#if 0
    actionsCtx.append({tr("3D"),
                       &SAKDebuggerPlugins::showPluin3D,
                       &SAKDebuggerPlugins::embedPluin3D});
#endif
    actionsCtx.append({tr("Charts"),
                       &SAKDebuggerPlugins::showPluinCharts,
                       &SAKDebuggerPlugins::embedPluinCharts});
    actionsCtx.append({tr("Transponders"),
                       &SAKDebuggerPlugins::showPluinTransponders,
                       &SAKDebuggerPlugins::embedPluinTransponders});
    actionsCtx.append({tr("Timed Sending"),
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
    QVector<QWidget*> pluginVector;
    pluginVector << m3d
                 //<< mCharts
                 << mTransponders
                 << mTimedSending
                 << mAutoResponse;
    for (int i = 0; i < pluginVector.count(); i++) {
        auto w = pluginVector.at(i);
        if ((w != mActiveWidgetInPanel) && (w != mActiveWidgetInDialog)) {
            w->deleteLater();
        }
    }
}

void SAKDebuggerPlugins::showPluin3D()
{
    showPluginDialog(m3d);
}

void SAKDebuggerPlugins::showPluinCharts()
{
    //showPluginDialog(mCharts);
}

void SAKDebuggerPlugins::showPluinTransponders()
{
    showPluginDialog(mTransponders);
}

void SAKDebuggerPlugins::showPluginAutoResponse()
{
    showPluginDialog(mAutoResponse);
}

void SAKDebuggerPlugins::showPluginRegularlySending()
{
    showPluginDialog(mTimedSending);
}

void SAKDebuggerPlugins::showPluginDialog(QWidget *contentWidget)
{
    clearPluginDialog();
    mActiveWidgetInDialog = contentWidget;
    mPluginDialog->layout()->addWidget(contentWidget);
    mPluginDialog->show();
    if (mActiveWidgetInPanel == mActiveWidgetInDialog) {
        cancelEmbedPlugin();
    }
}

void SAKDebuggerPlugins::embedPluin3D()
{
    embedPlugin(m3d);
}

void SAKDebuggerPlugins::embedPluinCharts()
{
    //embedPlugin(mCharts);
}

void SAKDebuggerPlugins::embedPluinTransponders()
{
    embedPlugin(mTransponders);
}

void SAKDebuggerPlugins::embedPluginAutoResponse()
{
    embedPlugin(mAutoResponse);
}

void SAKDebuggerPlugins::embedPluginRegularlySending()
{
    embedPlugin(mTimedSending);
}

void SAKDebuggerPlugins::embedPlugin(QWidget *contentWidget)
{
    clearPluginPanel();
    mActiveWidgetInPanel = contentWidget;
    mPanelWidget->layout()->addWidget(contentWidget);
    mTitleLabel->show();
    mPanelWidget->show();

    if (mActiveWidgetInPanel == mActiveWidgetInDialog) {
        clearPluginDialog();
    }
}

void SAKDebuggerPlugins::cancelEmbedPlugin()
{
    clearPluginPanel();
    mTitleLabel->hide();
    mPanelWidget->hide();
}

void SAKDebuggerPlugins::clearPluginDialog()
{
    for (int i = 0; i < mPluginDialog->layout()->count(); i++) {
        auto ret = mPluginDialog->layout()->takeAt(i);
        ret->widget()->setParent(Q_NULLPTR);
    }

    mActiveWidgetInDialog = Q_NULLPTR;
}

void SAKDebuggerPlugins::clearPluginPanel()
{
    for (int i = 0; i < mPanelWidget->layout()->count(); i++) {
        auto ret = mPanelWidget->layout()->takeAt(i);
        ret->widget()->setParent(Q_NULLPTR);
    }

    mActiveWidgetInPanel = Q_NULLPTR;
}
