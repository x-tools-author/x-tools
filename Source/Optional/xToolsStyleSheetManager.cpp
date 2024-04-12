/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source code
 * directory.
 **************************************************************************************************/
#include "xToolsStylesheetManager.h"

#include <QApplication>
#include <QDir>
#include <QFileInfoList>

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
    return xToolsSettings::instance()->value("themeName").toString();
}

void xToolsStyleSheetManager::setThemeName(const QString& themeName)
{
    xToolsSettings::instance()->setValue("themeName", themeName);
    setCurrentTheme(themeName);
    updateStylesheet();
#if 0
    qApp->setStyleSheet(styleSheet());
#endif
    qInfo() << "The stylesheet has been changed: " << this->currentTheme();
}

QMenu* xToolsStyleSheetManager::darkThemeMenu() const
{
    return m_darkThemeMenu;
}

QMenu* xToolsStyleSheetManager::lightThemeMenu() const
{
    return m_lightThemeMenu;
}

void xToolsStyleSheetManager::loadThemes()
{
    QDir dir = QDir(QApplication::applicationDirPath() + "/3rd_styles/qt_material/themes");
    QFileInfoList infoList = dir.entryInfoList(QDir::Files);
    for (const QFileInfo& info : infoList) {
        if (info.fileName().endsWith(".xml")) {
            if (info.fileName().contains("dark_")) {
                m_darkThemes.append(info.baseName());
            } else if (info.fileName().contains("light_")) {
                m_lightThemes.append(info.baseName());
            }
        }
    }

    auto setupActions = [this](const QStringList& themes, QMenu* menu, QActionGroup* actionGroup) {
        for (const QString& theme : themes) {
            QAction* action = new QAction(theme, menu);
            action->setCheckable(true);
            actionGroup->addAction(action);
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
