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
#include <QDialog>
#include <QSettings>
#include <QPushButton>
#include <QSqlDatabase>
#include <QPluginLoader>

#include "SAKDebuggerPluginTransponders.hh"
#include "SAKDebuggerPluginAutoResponse.hh"
#include "SAKDebuggerPluginTimedSending.hh"

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
    SAKDebuggerPluginTransponders *mTransponders;
    SAKDebuggerPluginAutoResponse *mAutoResponse;
    SAKDebuggerPluginTimedSending *mTimedSending;
private:
    QLabel *mTitleLabel;
    QWidget *mPanelWidget;
    QDialog *mPluginDialog;
    QWidget *mActiveWidgetInPanel;
    QWidget *mActiveWidgetInDialog;
    QVector<QAction*> mPluginActionVector;
    QVector<QAction*> mEmbedPluginActionVector;
private:
    void showPluinTransponders();
    void showPluginAutoResponse();
    void showPluginRegularlySending();
    void showPluginDialog(QWidget *contentWidget);

    void embedPluinTransponders();
    void embedPluginAutoResponse();
    void embedPluginRegularlySending();
    void embedPlugin(QWidget *contentWidget);
    void cancelEmbedPlugin();

    void clearPluginDialog();
    void clearPluginPanel();

#ifdef SAK_IMPORT_MODULE_PLUGIN
    void loadPlugin(QMenu *menu, QMenu *embedMenu);
    void addPluginToMenu(QMenu *menu, QString name, QWidget *ui, bool embed);
    void clearPlugins(QVector<QAction *> &actionVector);
#endif
signals:
    void bytesRead(QByteArray bytes);
    void bytesWritten(QByteArray bytes);

    void invokeWriteRawBytes(QString rawBytes);
    void invokeWriteCookedBytes(QByteArray bytes);
};

#endif // SAKDEBUGGERPLUGINS_HH
