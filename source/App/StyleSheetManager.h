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

namespace xTools {

class StyleSheetManager : public acss::QtAdvancedStylesheet
{
    Q_OBJECT
private:
    explicit StyleSheetManager(QObject* parent = Q_NULLPTR);
    StyleSheetManager(const StyleSheetManager&) = delete;
    StyleSheetManager& operator=(const StyleSheetManager&) = delete;

public:
    ~StyleSheetManager();

    static StyleSheetManager& singleton();
    QMenu* themeMenu() const;
    static void setEnableStyleSheetDefaultValue(bool enabled);
    static void setAwaysEnableStylesheet(bool enabled);

private:
    QMenu* m_themeMenu;
    QActionGroup* m_themeActionGroup;
    QMap<QString, QString> m_nameFriendlyNameMap;
    QMap<QString, QString> m_primaryColorMap;
    static bool m_enableStylesheet;
    static bool m_awaysEnableStylesheet;

private:
    QString themeName();
    void setThemeName(const QString& themeName);
    void updateApplicationStylesheet();

    void loadThemes();
    void updateActions();
    void updateActionIcon(QAction* action, const QString& color);

    bool enableStylesheet();
    void setEnableStylesheet(bool enable);
    void setupActions(const QStringList& themes, QMenu* menu, QActionGroup* group);

    void setApplicationStylesheetEnabled(bool enable);
};

} // namespace xTools
