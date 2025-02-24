/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "qssmgr.h"

#include <QApplication>
#include <QDir>
#include <QFileInfoList>
#include <QMessageBox>
#include <QPainter>
#include <QStandardPaths>

bool QssMgr::m_enableStylesheet = false;
bool QssMgr::m_awaysEnableStylesheet = false;
QssMgr::QssMgr(QObject* parent)
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

    if (!m_settings) {
        QString fileName = QStandardPaths::writableLocation(QStandardPaths::AppConfigLocation);
        fileName += "/xQSS.ini";
        m_settings = new QSettings(fileName, QSettings::IniFormat, this);
    }

    QString fileName = m_settings->fileName();
    QString settingsPath = fileName.left(fileName.lastIndexOf('/') + 1);

    QDir dir(settingsPath);
    if (!dir.exists("output")) {
        dir.mkdir("output");
    }

    QString appDir = QApplication::applicationDirPath();
    setStylesDirPath(appDir + "/3rd_styles");
    setOutputDirPath(settingsPath + "/output");
    setCurrentStyle("qt_material");
    setCurrentTheme(themeName());

    qInfo() << "The styles directory is:" << stylesDirPath();
    qInfo() << "The output directory is:" << outputDirPath();
    qInfo() << "The current style is:" << currentStyle();
    qInfo() << "The current theme is:" << currentTheme();

    m_themeActionGroup = new QActionGroup(this);
    m_themeMenu = new QMenu(tr("Application Stylesheet"));
    if (m_awaysEnableStylesheet) {
        updateApplicationStylesheet();
    } else {
        bool checked = enableStylesheet();
        QAction* action = new QAction(tr("Enable Stylesheet"));
        action->setCheckable(true);
        action->setChecked(checked);
        if (checked) {
            updateApplicationStylesheet();
        }

        connect(action, &QAction::triggered, this, &QssMgr::setApplicationStylesheetEnabled);

        m_themeMenu->addAction(action);
        m_themeMenu->addSeparator();
    }

    loadThemes();
}

QssMgr::~QssMgr() {}

QssMgr& QssMgr::singleton()
{
    static QssMgr instance;
    return instance;
}

QMenu* QssMgr::themeMenu() const
{
    return m_themeMenu;
}

void QssMgr::setEnableStyleSheetDefaultValue(bool enabled)
{
    m_enableStylesheet = enabled;
}

void QssMgr::setAwaysEnableStylesheet(bool enabled)
{
    m_awaysEnableStylesheet = enabled;
}

QString QssMgr::themeName()
{
    QString ret = m_settings->value("themeName").toString();
    if (ret.isEmpty()) {
        ret = QString("dark_blue");
    }

    return ret;
}

void QssMgr::setThemeName(const QString& themeName)
{
    m_settings->setValue("themeName", themeName);
    setCurrentTheme(themeName);
    if (enableStylesheet()) {
        updateStylesheet();
    }
}

void QssMgr::updateApplicationStylesheet()
{
    if (qApp) {
        updateStylesheet();
        qApp->setStyleSheet(styleSheet());
    }
}

void QssMgr::loadThemes()
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
    m_themeMenu->addSeparator();
    setupActions(m_lightThemes, m_themeMenu, m_themeActionGroup);
}

void QssMgr::updateActions()
{
    for (QAction* action : m_themeActionGroup->actions()) {
        QString theme = action->data().toString();
        QString color = m_primaryColorMap.value(theme);
        updateActionIcon(action, color);
    }
}

void QssMgr::updateActionIcon(QAction* action, const QString& color)
{
    if (action->isChecked()) {
        action->setIcon(QIcon(":/res/Icons/IconCheck.svg"));
    } else {
        QPixmap pixmap(64, 64);
        pixmap.fill(color);
#if 0
        QPainter painter(&pixmap);
        QRectF tmp = pixmap.rect() - QMargins(16, 16, 16, 16);
        painter.setPen(Qt::black);
        painter.setBrush(Qt::white);
        painter.drawRect(tmp);
        painter.end();
#endif
        action->setIcon(QIcon(pixmap));
    }
}

bool QssMgr::enableStylesheet()
{
    return m_settings->value("enableStylesheet", m_enableStylesheet).toBool();
}

void QssMgr::setEnableStylesheet(bool enable)
{
    m_settings->setValue("enableStylesheet", enable);
}

void QssMgr::setupActions(const QStringList& themes, QMenu* menu, QActionGroup* group)
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

void QssMgr::setApplicationStylesheetEnabled(bool enable)
{
    setEnableStylesheet(enable);
    for (auto action : m_themeActionGroup->actions()) {
        if (action->isChecked() && enable) {
            emit action->triggered();
            break;
        }
    }
}
