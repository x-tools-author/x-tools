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
#include <QPainter>

#include "Common/CommonUI/xTools.h"
#include "xToolsSettings.h"

xToolsStyleSheetManager::xToolsStyleSheetManager(QObject* parent)
    : acss::QtAdvancedStylesheet(parent)
{
    m_nameFriendlyNameMap.insert("dark_amber", tr("Dark Amber"));
    m_nameFriendlyNameMap.insert("dark_blue", tr("Dark Blue"));
    m_nameFriendlyNameMap.insert("dark_cyan", tr("Dark Cyan"));
    m_nameFriendlyNameMap.insert("dark_lightgreen", tr("Dark Light Green"));
    m_nameFriendlyNameMap.insert("dark_pink", tr("Dark Pink"));
    m_nameFriendlyNameMap.insert("dark_purple", tr("Dark Purple"));
    m_nameFriendlyNameMap.insert("dark_red", tr("Dark Red"));
    m_nameFriendlyNameMap.insert("dark_teal", tr("Dark Teal"));
    m_nameFriendlyNameMap.insert("dark_yellow", tr("Dark Yellow"));

    m_nameFriendlyNameMap.insert("light_amber", tr("Light Amber"));
    m_nameFriendlyNameMap.insert("light_blue", tr("Light Blue"));
    m_nameFriendlyNameMap.insert("light_cyan", tr("Light Cyan"));
    m_nameFriendlyNameMap.insert("light_cyan_500", tr("Light Cyan(500)"));
    m_nameFriendlyNameMap.insert("light_lightgreen", tr("Light Light Green"));
    m_nameFriendlyNameMap.insert("light_pink", tr("Light Pink"));
    m_nameFriendlyNameMap.insert("light_purple", tr("Light Purple"));
    m_nameFriendlyNameMap.insert("light_red", tr("Light Red"));
    m_nameFriendlyNameMap.insert("light_teal", tr("Light Teal"));
    m_nameFriendlyNameMap.insert("light_yellow", tr("Light Yellow"));

    m_primaryColorMap.insert("dark_amber", "#ffd740");
    m_primaryColorMap.insert("dark_blue", "#448aff");
    m_primaryColorMap.insert("dark_cyan", "#4dd0e1");
    m_primaryColorMap.insert("dark_lightgreen", "#8bc34a");
    m_primaryColorMap.insert("dark_pink", "#ff4081");
    m_primaryColorMap.insert("dark_purple", "#ab47bc");
    m_primaryColorMap.insert("dark_red", "#ff1744");
    m_primaryColorMap.insert("dark_teal", "#1de9b6");
    m_primaryColorMap.insert("dark_yellow", "#ffff00");

    m_primaryColorMap.insert("light_amber", "#ffc400");
    m_primaryColorMap.insert("light_blue", "#2979ff");
    m_primaryColorMap.insert("light_cyan", "#00e5ff");
    m_primaryColorMap.insert("light_cyan_500", "#00bcd4");
    m_primaryColorMap.insert("light_lightgreen", "#64dd17");
    m_primaryColorMap.insert("light_pink", "#ff4081");
    m_primaryColorMap.insert("light_purple", "#e040fb");
    m_primaryColorMap.insert("light_red", "#ff1744");
    m_primaryColorMap.insert("light_teal", "#1de9b6");
    m_primaryColorMap.insert("light_yellow", "#ffea00");

    QDir dir(xToolsSettings::instance()->settingsPath());
    if (!dir.exists("output")) {
        dir.mkdir("output");
    }

    QString appDir = QApplication::applicationDirPath();
    setStylesDirPath(appDir + "/3rd_styles");
    setOutputDirPath(xToolsSettings::instance()->settingsPath() + "/output");
    setCurrentStyle("qt_material");
    setCurrentTheme(themeName());

    qInfo() << "The styles directory is:" << stylesDirPath();
    qInfo() << "The output directory is:" << outputDirPath();
    qInfo() << "The current style is:" << currentStyle();
    qInfo() << "The current theme is:" << currentTheme();

    bool checked = enableStylesheet();
    QAction* action = new QAction(tr("Enable Stylesheet"));
    action->setCheckable(true);
    action->setChecked(checked);
    if (checked) {
        updateApplicationStylesheet();
    }

    connect(action,
            &QAction::triggered,
            this,
            &xToolsStyleSheetManager::setApplicationStylesheetEnabled);

    m_themeActionGroup = new QActionGroup(this);
    m_themeMenu = new QMenu(tr("Application Stylesheet"));
    m_themeMenu->addAction(action);
    m_themeMenu->addSeparator();

    loadThemes();
}

xToolsStyleSheetManager::~xToolsStyleSheetManager() {}

xToolsStyleSheetManager& xToolsStyleSheetManager::singleton()
{
    static xToolsStyleSheetManager instance;
    return instance;
}

QMenu* xToolsStyleSheetManager::themeMenu() const
{
    return m_themeMenu;
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
    if (enableStylesheet()) {
        updateStylesheet();
    }
}

void xToolsStyleSheetManager::updateApplicationStylesheet()
{
    if (qApp) {
        updateStylesheet();
        qApp->setStyleSheet(styleSheet());
    }
}

void xToolsStyleSheetManager::loadThemes()
{
    QStringList themeList = themes();
    QStringList m_darkThemes;
    QStringList m_lightThemes;
    for (QString& theme : themeList) {
        if (theme.contains("dark_")) {
            m_darkThemes.append(theme);
        } else if (theme.contains("light_")) {
            m_lightThemes.append(theme);
        }
    }

    setupActions(m_darkThemes, m_themeMenu, m_themeActionGroup);
    setupActions(m_lightThemes, m_themeMenu, m_themeActionGroup);
}

void xToolsStyleSheetManager::updateActions()
{
    for (QAction* action : m_themeActionGroup->actions()) {
        QString theme = action->data().toString();
        QString color = m_primaryColorMap.value(theme);
        updateActionIcon(action, color);
    }
}

void xToolsStyleSheetManager::updateActionIcon(QAction* action, const QString& color)
{
    QPixmap pixmap(64, 64);
    pixmap.fill(color);
    if (action->isChecked()) {
        QPainter painter(&pixmap);
        QRectF tmp = pixmap.rect() - QMargins(16, 16, 16, 16);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::white);
        painter.drawRect(tmp);
        painter.end();
    }
    action->setIcon(QIcon(pixmap));
}

bool xToolsStyleSheetManager::enableStylesheet()
{
    xToolsSettings* settings = xToolsSettings::instance();
    return settings->value("enableStylesheet", false).toBool();
}

void xToolsStyleSheetManager::setEnableStylesheet(bool enable)
{
    xToolsSettings* settings = xToolsSettings::instance();
    settings->setValue("enableStylesheet", enable);
}

void xToolsStyleSheetManager::setupActions(const QStringList& themes,
                                           QMenu* menu,
                                           QActionGroup* group)
{
    for (const QString& theme : themes) {
        bool isValidName = m_nameFriendlyNameMap.contains(theme);
        QString txt = isValidName ? this->m_nameFriendlyNameMap.value(theme) : theme;
        QString color = this->m_primaryColorMap.value(theme);
        QAction* action = new QAction(txt, menu);
        action->setData(theme);
        action->setCheckable(true);
        group->addAction(action);
        menu->addAction(action);
        if (theme == themeName()) {
            action->setChecked(true);
        }

        updateActionIcon(action, color);

        connect(action, &QAction::triggered, this, [this, theme] {
            updateActions();
            setThemeName(theme);
            if (enableStylesheet()) {
                updateApplicationStylesheet();
            } else {
                QMessageBox::warning(nullptr, tr("Warning"), tr("Please enable stylesheet first!"));
            }
        });
    }
}

void xToolsStyleSheetManager::setApplicationStylesheetEnabled(bool enable)
{
    setEnableStylesheet(enable);
    for (auto action : m_themeActionGroup->actions()) {
        if (action->isChecked() && enable) {
            emit action->triggered();
            break;
        }
    }

    if (!enable) {
        try2rebootApp();
    }
}
