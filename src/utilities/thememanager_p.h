/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include <private/qobject_p.h>

#include <QActionGroup>
#include <QApplication>
#include <QMenu>
#include <QSettings>
#include <QStyleHints>

#if QT_VERSION >= QT_VERSION_CHECK(6, 8, 0)
#define xEnableColorScheme 1
#else
#define xEnableColorScheme 0
#endif

#ifdef Q_OS_WIN32
#include <dwmapi.h>

#include <QAction>
#include <QColor>
#include <QPalette>
#include <QSysInfo>
#include <QWidget>
#include <QWindow>
#endif

#include "thememanager.h"

namespace xTools {

class ThemeManagerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(ThemeManager)
public:
    explicit ThemeManagerPrivate() {}
    ~ThemeManagerPrivate() override {}

    QMenu *themeMenu()
    {
#if xEnableColorScheme
        static QActionGroup *actionGroup{Q_NULLPTR};
        if (actionGroup) {
            return colorSchemeMenu;
        }

        Q_Q(ThemeManager);
        actionGroup = new QActionGroup(q);
        colorSchemeMenu = new QMenu(QObject::tr("Color Scheme"));
        QMap<Qt::ColorScheme, QString> colorSchemeMap;
        colorSchemeMap.insert(Qt::ColorScheme::Dark, QObject::tr("Dark"));
        colorSchemeMap.insert(Qt::ColorScheme::Light, QObject::tr("Light"));
        colorSchemeMap.insert(Qt::ColorScheme::Unknown, QObject::tr("System"));

        for (auto it = colorSchemeMap.begin(); it != colorSchemeMap.end(); ++it) {
            auto action = new QAction(it.value(), q);
            action->setCheckable(true);
            actionGroup->addAction(action);
            colorSchemeMenu->addAction(action);

            if (it.key() == qApp->styleHints()->colorScheme()) {
                action->setChecked(true);
            }

            Qt::ColorScheme colorScheme = it.key();
            QObject::connect(action, &QAction::triggered, q, [=]() {
                if (settings) {
                    settings->setValue(keyColorScheme, static_cast<int>(colorScheme));
                }

                updateApplicationColor();
                emit q->colorSchemeChanged();
            });

            updateApplicationColor();
        }
#endif
        return colorSchemeMenu;
    }

    void updateWindowCaption()
    {
        QList<QWidget *> topLevelWidgets = QApplication::topLevelWidgets();
        for (QWidget *widget : const_cast<QList<QWidget *> &>(topLevelWidgets)) {
            updateWindowCaption(widget);
        }
    }

    void updateWindowCaption(QWidget *widget)
    {
#ifdef Q_OS_WIN32
        bool isWindows11 = QSysInfo::productVersion().contains("11");
        bool isWindows10 = QSysInfo::productVersion().contains("10");
        if (!isWindows10 && !isWindows11) {
            return;
        }

        QPalette palette = qApp->palette();
        QWindow *window = widget->windowHandle();
        if (!window) {
            return;
        }

        QColor c = palette.color(QPalette::Window);
        COLORREF colorref = c.red() | (c.green() << 8) | (c.blue() << 16);
        // The value of DWMWA_CAPTION_COLOR is 35, visit the website for more information:
        // https://learn.microsoft.com/zh-cn/windows/win32/api/dwmapi/ne-dwmapi-dwmwindowattribute
        DwmSetWindowAttribute((HWND) window->winId(), 35, &colorref, sizeof(colorref));
#endif
    }

    void updateApplicationColor()
    {
#if xEnableColorScheme
        Qt::ColorScheme def = qApp->styleHints()->colorScheme();
        int colorScheme = static_cast<int>(def);
        if (settings) {
            colorScheme = settings->value(keyColorScheme, static_cast<int>(def)).toInt();
        }
        Qt::ColorScheme cookedColorScheme = static_cast<Qt::ColorScheme>(colorScheme);
        QStyleHints *hints = qApp->styleHints();
        hints->setColorScheme(cookedColorScheme);
#endif
        updateWindowCaption();
    }

public:
    QSettings *settings{nullptr};
    QMenu *colorSchemeMenu{nullptr};
    const QString keyColorScheme{"Application/ColorScheme"};
};

} // namespace xTools
