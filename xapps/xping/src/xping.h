/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QApplication>
#include <QMutex>
#include <QSettings>
#include <QSplashScreen>

#include "sortfilterproxymodel.h"
#include "tablemodel.h"

class Settings;
class xPing : public QApplication
{
    Q_OBJECT
    Q_PROPERTY(QVariant proxyModel READ proxyModel NOTIFY proxyModelChanged FINAL)
    Q_PROPERTY(int total READ total NOTIFY totalChanged FINAL)
    Q_PROPERTY(int active READ active NOTIFY activeChanged FINAL)
    Q_PROPERTY(int finished READ finished NOTIFY finishedChanged FINAL)
    Q_PROPERTY(bool beepEnable READ beepEnable WRITE setBeepEnable NOTIFY beepEnableChanged FINAL)
public:
    explicit xPing(int &argc, char **argv);
    ~xPing() override;

    static Settings *settings();
    static QString settingsPath();

public:
    Q_INVOKABLE void startPing(const QString &startIp, const QString &endIp);
    Q_INVOKABLE QString checkParameters(const QString &startIp, const QString &endIp);
    Q_INVOKABLE void updateTableModel(const QString &ip, bool isOnline, const QString &description);
    Q_INVOKABLE void setFilterText(const QString &text);
    Q_INVOKABLE void setFilterTextDelta();
    Q_INVOKABLE QString version();

    Q_INVOKABLE QString onlineText();
    Q_INVOKABLE QString offlineText();
    Q_INVOKABLE QString language();
    Q_INVOKABLE void setupLanguage();
    Q_INVOKABLE void setupLanguageFile(const QString &qmFile);

    QSplashScreen *splashScreen();
    Q_INVOKABLE void showSplashScreenMessage(const QString &msg);
    Q_INVOKABLE void openSettingsFileDir();

signals:
    void pingFinished();
    void languageChanged();

private:
    TableModel *m_tableModel{nullptr};
    TableModel *m_oldTableModel{nullptr};
    QMutex m_updateTableModelMutex;
    QString m_filterText;

private:
    void updateTableModelInner(const QString &ip, bool isOnline, const QString &description);
    void updateTableModelInnerCompare(const QString &ip, bool isOnline);

private:
    SortFilterProxyModel *m_proxyModel{nullptr};
    QVariant proxyModel() const { return QVariant::fromValue(m_proxyModel); }
    Q_SIGNAL void proxyModelChanged();

    int m_total{0};
    int total() const { return m_total; }
    Q_SIGNAL void totalChanged();

    int m_active{0};
    int active() const { return m_active; }
    Q_SIGNAL void activeChanged();

    int m_finished{0};
    int finished() const { return m_finished; }
    Q_SIGNAL void finishedChanged();

    bool beepEnable();
    void setBeepEnable(bool enable);
    Q_SIGNAL void beepEnableChanged();
};
