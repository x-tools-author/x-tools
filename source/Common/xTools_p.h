/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#pragma once

#include <private/qobject_p.h>
#include <QAction>
#include <QActionGroup>
#include <QApplication>
#include <QClipboard>
#include <QDebug>
#include <QDesktopServices>
#include <QFile>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QLabel>
#include <QMenu>
#include <QMenuBar>
#include <QMessageBox>
#include <QPainter>
#include <QProcess>
#include <QScreen>
#include <QSettings>
#include <QSplashScreen>
#include <QStandardPaths>
#include <QStyle>
#include <QStyleFactory>
#include <QStyleHints>
#include <QSvgRenderer>
#include <QTimer>
#include <QUrl>

#include "xTools.h"

namespace xTools {

class xToolsPrivate : QObjectPrivate
{
    Q_DECLARE_PUBLIC(xTools);

public:
    xToolsPrivate()
    {
        QStandardPaths::StandardLocation type = QStandardPaths::AppConfigLocation;
        QString path = QStandardPaths::writableLocation(type);
        QString fileName = QString("%1/%2v7.ini").arg(path, QCoreApplication::applicationName());
        m_settings = new QSettings(fileName, QSettings::IniFormat, parent);

        m_languageFlagNameMap.insert("zh_CN", "简体中文");
        m_languageFlagNameMap.insert("en", "English");
#if 0
        m_languageFlagNameMap.insert("zh_TW", "繁體中文");
        m_languageFlagNameMap.insert("ar", "العربية");
        m_languageFlagNameMap.insert("cs", "Čeština");
        m_languageFlagNameMap.insert("da", "Dansk");
        m_languageFlagNameMap.insert("de", "Deutsch");
        m_languageFlagNameMap.insert("es", "Español");
        m_languageFlagNameMap.insert("fa", "فارسی");
        m_languageFlagNameMap.insert("fi", "Suomi");
        m_languageFlagNameMap.insert("fr", "Français");
        m_languageFlagNameMap.insert("he", "עִבְרִית");
        m_languageFlagNameMap.insert("uk", "українська мова");
        m_languageFlagNameMap.insert("it", "Italiano");
        m_languageFlagNameMap.insert("ja", "日本语");
        m_languageFlagNameMap.insert("ko", "한글");
        m_languageFlagNameMap.insert("lt", "Lietuvių kalba");
        m_languageFlagNameMap.insert("pl", "Polski");
        m_languageFlagNameMap.insert("pt", "Português");
        m_languageFlagNameMap.insert("ru", "русский язык");
        m_languageFlagNameMap.insert("sk", "Slovenčina");
        m_languageFlagNameMap.insert("sl", "Slovenščina");
        m_languageFlagNameMap.insert("sv", "Svenska");
#endif
    }

public:
    QString settingsPath()
    {
        QString settingsFile(m_settings->fileName());
        QString path = settingsFile.left(settingsFile.lastIndexOf("/"));
        return path;
    }

    bool m_enableSplashScreen = true;
    QSplashScreen *m_splashScreen{nullptr};
    QString m_appFriendlyName;
    QSettings *m_settings;
    QMap<QString, QString> m_languageFlagNameMap;
    QStringList m_appSupportedLanguagePrefixes;
};

} // namespace xTools
