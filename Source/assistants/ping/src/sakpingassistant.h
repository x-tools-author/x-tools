/***************************************************************************************************
 * Copyright 2024 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#ifndef SAKPINGASSISTANT_H
#define SAKPINGASSISTANT_H

#include <QLabel>
#include <QSettings>
#include <QTimer>
#include <QToolBar>
#include <QWidget>

#include "xToolsMainWindow.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class SAKPingAssistant;
}
QT_END_NAMESPACE

class SAKPingAssistant : public xToolsMainWindow
{
    Q_OBJECT

public:
    Q_INVOKABLE SAKPingAssistant(QWidget *parent = nullptr);
    ~SAKPingAssistant();

    void emitPingStarted(const QString &ip);
    void emitPingFinished(const QString &ip, bool isOnline, const QString &description);

signals:
    void pingStarted(const QString &ip);
    void pingFinished(const QString &ip, bool isOnline, const QString &description);

private:
    enum ShowModel { ShowAll, ShowOnline };

    struct
    {
        const QString beginIp = "beginIp";
        const QString endIp = "endIp";
        const QString showModel = "showModel";
    } m_keyCtx;

private:
    Ui::SAKPingAssistant *ui;

    QTimer *m_playTimer;
    QString m_beginIp;
    QString m_endIp;
    int m_currentRow;
    bool m_pausing;
    QToolBar *m_runToolBar;
    QToolBar *m_showToolBar;
    QToolBar *m_settingToolBar;
    QSettings *m_settings;
    int m_finishedCount;
    int m_showModel;
    QList<QPair<QString, bool>> m_preScanResult;
    QLabel *m_progressStatus;

    QAction *m_playAction;
    QAction *m_pauseAction;
    QAction *m_stopAction;

    QString m_buildDate;
    QString m_buildTime;

private:
    void init();
    void initTableWidget();
    void initToolBar();
    void initRunToolBar();
    void initSettingToolBar();
    void initShowToolBar();

    void play();
    void pause();
    void stop();

    bool isValidIp();
    int ipCount();
    QString ip(int row);
    int row(const QString &ip);
    void updateRowVisible();
    void updateProgressStatus();

    void onPlayTimerTimeout();
    void onPingStarted(const QString &ip);
    void onPingFinished(const QString &ip, bool is_online, const QString &description);
};
#endif // MAINWINDOW_H
