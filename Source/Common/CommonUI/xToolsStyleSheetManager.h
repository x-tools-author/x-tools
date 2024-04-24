/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded in "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QAction>
#include <QActionGroup>
#include <QMenu>

#include <QtAdvancedStylesheet.h>

class xToolsStyleSheetManager : public acss::QtAdvancedStylesheet
{
    Q_OBJECT
private:
    explicit xToolsStyleSheetManager(QObject* parent = Q_NULLPTR);

public:
    ~xToolsStyleSheetManager();
    xToolsStyleSheetManager(const xToolsStyleSheetManager&) = delete;
    xToolsStyleSheetManager& operator=(const xToolsStyleSheetManager&) = delete;

    static xToolsStyleSheetManager& instance();

    QString themeName();
    void setThemeName(const QString& themeName);
    QMenu* darkThemeMenu() const;
    QMenu* lightThemeMenu() const;
    void updateApplicationStylesheet();

signals:
    void applicationStylesheetChanged();

private:
    QStringList m_darkThemes;
    QStringList m_lightThemes;
    QMenu* m_darkThemeMenu;
    QMenu* m_lightThemeMenu;
    QActionGroup* m_darkThemeActionGroup;
    QActionGroup* m_lightThemeActionGroup;
    QMap<QString, QString> m_nameFriendlyNameMap;
    QMap<QString, QString> m_primaryColorMap;

private:
    void loadThemes();
    void updateActions();
    void updateActionIcon(QAction* action, const QString& color);
};
