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
#if !X_BUILD_FOR_WIN_XP
#include <dwmapi.h>

#include <QAction>
#include <QColor>
#include <QPalette>
#include <QSysInfo>
#include <QWidget>
#include <QWindow>
#endif
#endif

#ifdef Q_OS_LINUX
#include <QColor>
#include <QPalette>
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
#if !X_BUILD_FOR_WIN_XP
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
        static bool firstTime{true};
        if (firstTime) {
            firstTime = false;
            qInfo("The current color scheme is %d.", static_cast<int>(cookedColorScheme));
        }

#if 0   //def Q_OS_LINUX
        // On Linux, setColorScheme alone may not be enough, we need to set QPalette
        QPalette palette;
        if (cookedColorScheme == Qt::ColorScheme::Dark) {
            // Dark mode palette
            palette.setColor(QPalette::Window, QColor(53, 53, 53));
            palette.setColor(QPalette::WindowText, Qt::white);
            palette.setColor(QPalette::Base, QColor(25, 25, 25));
            palette.setColor(QPalette::AlternateBase, QColor(53, 53, 53));
            palette.setColor(QPalette::ToolTipBase, Qt::white);
            palette.setColor(QPalette::ToolTipText, Qt::white);
            palette.setColor(QPalette::Text, Qt::white);
            palette.setColor(QPalette::Button, QColor(53, 53, 53));
            palette.setColor(QPalette::ButtonText, Qt::white);
            palette.setColor(QPalette::BrightText, Qt::red);
            palette.setColor(QPalette::Link, QColor(42, 130, 218));
            palette.setColor(QPalette::Highlight, QColor(42, 130, 218));
            palette.setColor(QPalette::HighlightedText, Qt::black);
            qApp->setPalette(palette);
        } else if (cookedColorScheme == Qt::ColorScheme::Light) {
            // Light mode palette
            palette.setColor(QPalette::Window, Qt::white);
            palette.setColor(QPalette::WindowText, Qt::black);
            palette.setColor(QPalette::Base, Qt::white);
            palette.setColor(QPalette::AlternateBase, QColor(240, 240, 240));
            palette.setColor(QPalette::ToolTipBase, Qt::black);
            palette.setColor(QPalette::ToolTipText, Qt::black);
            palette.setColor(QPalette::Text, Qt::black);
            palette.setColor(QPalette::Button, QColor(240, 240, 240));
            palette.setColor(QPalette::ButtonText, Qt::black);
            palette.setColor(QPalette::BrightText, Qt::red);
            palette.setColor(QPalette::Link, QColor(0, 0, 255));
            palette.setColor(QPalette::Highlight, QColor(0, 120, 215));
            palette.setColor(QPalette::HighlightedText, Qt::white);
            qApp->setPalette(palette);
        } else {
            // System default
            qApp->setPalette(QApplication::style()->standardPalette());
        }
#endif
#endif
        updateWindowCaption();
    }

public:
    QSettings *settings{nullptr};
    QMenu *colorSchemeMenu{nullptr};
    const QString keyColorScheme{"Application/ColorScheme"};
};

} // namespace xTools
