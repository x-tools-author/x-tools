/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsStyleSheetManager.h"

#include <QApplication>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>

#include "xToolsSettings.h"

xToolsStyleSheetManager::xToolsStyleSheetManager(QObject* parent)
{
    QDir dir(xToolsSettings::instance()->settingsPath());
    if (!dir.exists("output")) {
        dir.mkdir("output");
    }

    QString appDir = QApplication::applicationDirPath();
    setStylesDirPath(appDir + "/3rd_styles");
    setOutputDirPath(xToolsSettings::instance()->settingsPath() + "/output");
    setCurrentStyle("qt_material");
    setCurrentTheme(themeName());
    updateStylesheet();

    qInfo() << "The styles directory is:" << stylesDirPath();
    qInfo() << "The output directory is:" << outputDirPath();
    qInfo() << "The current style is:" << currentStyle();
    qInfo() << "The current theme is:" << currentTheme();

    m_darkThemeMenu = new QMenu(tr("Dark Theme"));
    m_lightThemeMenu = new QMenu(tr("Light Theme"));
    m_darkThemeActionGroup = new QActionGroup(this);
    m_lightThemeActionGroup = new QActionGroup(this);

    loadThemes();
}

xToolsStyleSheetManager::~xToolsStyleSheetManager() {}

xToolsStyleSheetManager& xToolsStyleSheetManager::instance()
{
    static xToolsStyleSheetManager instance;
    return instance;
}

QString xToolsStyleSheetManager::themeName()
{
    QString ret = xToolsSettings::instance()->value("themeName").toString();
    if (ret.isEmpty()) {
        ret = QString("dark_blue");
    }

    return ret;
}

void xToolsStyleSheetManager::setThemeName(const QString& themeName)
{
    xToolsSettings::instance()->setValue("themeName", themeName);
    setCurrentTheme(themeName);
    updateStylesheet();
}

QMenu* xToolsStyleSheetManager::darkThemeMenu() const
{
    return m_darkThemeMenu;
}

QMenu* xToolsStyleSheetManager::lightThemeMenu() const
{
    return m_lightThemeMenu;
}

void xToolsStyleSheetManager::updateApplicationStylesheet()
{
    qApp->setStyleSheet(styleSheet());
    emit applicationStylesheetChanged();
}

void xToolsStyleSheetManager::loadThemes()
{
    static QMap<QString, QString> nameFriendlyNameMap;
    if (nameFriendlyNameMap.isEmpty()) {
        nameFriendlyNameMap.insert("dark_amber", tr("Dark Amber"));
        nameFriendlyNameMap.insert("dark_blue", tr("Dark Blue"));
        nameFriendlyNameMap.insert("dark_cyan", tr("Dark Cyan"));
        nameFriendlyNameMap.insert("dark_lightgreen", tr("Dark Light Green"));
        nameFriendlyNameMap.insert("dark_pink", tr("Dark Pink"));
        nameFriendlyNameMap.insert("dark_purple", tr("Dark Purple"));
        nameFriendlyNameMap.insert("dark_red", tr("Dark Red"));
        nameFriendlyNameMap.insert("dark_teal", tr("Dark Teal"));
        nameFriendlyNameMap.insert("dark_yellow", tr("Dark Yellow"));

        nameFriendlyNameMap.insert("light_amber", tr("Light Amber"));
        nameFriendlyNameMap.insert("light_blue", tr("Light Blue"));
        nameFriendlyNameMap.insert("light_cyan", tr("Light Cyan"));
        nameFriendlyNameMap.insert("light_cyan_500", tr("Light Cyan(500)"));
        nameFriendlyNameMap.insert("light_lightgreen", tr("Light Light Green"));
        nameFriendlyNameMap.insert("light_pink", tr("Light Pink"));
        nameFriendlyNameMap.insert("light_purple", tr("Light Purple"));
        nameFriendlyNameMap.insert("light_red", tr("Light Red"));
        nameFriendlyNameMap.insert("light_teal", tr("Light Teal"));
        nameFriendlyNameMap.insert("light_yellow", tr("Light Yellow"));
    }

    QStringList themeList = themes();
    for (QString& theme : themeList) {
        if (theme.contains("dark_")) {
            m_darkThemes.append(theme);
        } else if (theme.contains("light_")) {
            m_lightThemes.append(theme);
        }
    }

    auto setupActions = [&, this](const QStringList& themes, QMenu* menu, QActionGroup* group) {
        for (const QString& theme : themes) {
            QString txt = nameFriendlyNameMap.contains(theme) ? nameFriendlyNameMap[theme] : theme;
            QAction* action = new QAction(txt, menu);
            action->setCheckable(true);
            group->addAction(action);
            menu->addAction(action);
            if (theme == themeName()) {
                action->setChecked(true);
            }
            connect(action, &QAction::triggered, this, [this, theme] { setThemeName(theme); });
        }
    };

    setupActions(m_darkThemes, m_darkThemeMenu, m_darkThemeActionGroup);
    setupActions(m_lightThemes, m_lightThemeMenu, m_lightThemeActionGroup);
}
