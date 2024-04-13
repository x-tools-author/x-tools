/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QLabel>
#include <QLineEdit>
#include <QProgressBar>
#include <QSettings>
#include <QSpinBox>
#include <QStatusBar>
#include <QTimer>
#include <QToolBar>
#include <QWidget>

QT_BEGIN_NAMESPACE
namespace Ui {
class xToolsPingAssistant;
}
QT_END_NAMESPACE

class xToolsPingAssistant : public QWidget
{
    Q_OBJECT

public:
    Q_INVOKABLE xToolsPingAssistant(QWidget *parent = nullptr);
    ~xToolsPingAssistant();

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
        const QString timeout = "timeout";
    } m_keyCtx;

private:
    Ui::xToolsPingAssistant *ui;

    QTimer *m_playTimer;
    QString m_beginIp;
    QString m_endIp;
    int m_currentRow;
    bool m_pausing;
    QSettings *m_settings;
    int m_finishedCount;
    int m_showModel;
    QList<QPair<QString, bool>> m_preScanResult;
    QLabel *m_progressStatus;
    QStatusBar *m_statusBar;
    QToolBar *m_toolBar;
    QProgressBar *m_progressBar;
    int m_timeout;

    QAction *m_playAction;
    QAction *m_pauseAction;
    QAction *m_stopAction;

    QString m_buildDate;
    QString m_buildTime;

    QLineEdit *m_beginIpLineEdit;
    QLineEdit *m_endIpLineEdit;
    QSpinBox *m_timeoutSpinBox;

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
