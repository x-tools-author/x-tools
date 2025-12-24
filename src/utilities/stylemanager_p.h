/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <private/qobject_p.h>

#include <QActionGroup>
#include <QApplication>
#include <QDebug>
#include <QMenu>
#include <QSettings>
#include <QStyle>
#include <QStyleFactory>

#if QT_VERSION >= QT_VERSION_CHECK(6, 1, 0)
#define xDefaultStyleName QApplication::style()->name()
#else
#define xDefaultStyleName QString("fusion")
#endif

namespace xTools {

class StyleManagerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(StyleManager)
public:
    StyleManagerPrivate() {}
    ~StyleManagerPrivate() {}

    void setupAppStyle()
    {
        if (!settings) {
            return;
        }

        QString defaultStyle = xDefaultStyleName;
        QString style = settings->value(keyStyle, defaultStyle).toString();
        if (QStyleFactory::keys().contains(style, Qt::CaseInsensitive) == false) {
            style = defaultStyle;
        }

        for (QString& key : QStyleFactory::keys()) {
            if (key.toLower() == style.toLower()) {
                QApplication::setStyle(QStyleFactory::create(style));
            }
        }

        qInfo() << "The current application style is:" << style;
    }

    void initializeStyleMenu()
    {
        Q_Q(StyleManager);
        styleMenu = new QMenu();
        static QActionGroup* appStyleActionGroup = new QActionGroup(styleMenu);
        for (QString& key : QStyleFactory::keys()) {
            auto* action = new QAction(key, styleMenu);
            action->setCheckable(true);
            appStyleActionGroup->addAction(action);
            QString style = settings->value(keyStyle, xDefaultStyleName).toString();
            if (key.toUpper() == style.toUpper()) {
                action->setChecked(true);
            }

            QObject::connect(action, &QAction::triggered, action, [=]() {
                if (!this->settings) {
                    return;
                }

                settings->setValue(keyStyle, key.toLower());
                emit q->styleChanged();
            });
        }

        styleMenu->addActions(appStyleActionGroup->actions());
    }

public:
    QMenu* styleMenu{nullptr};
    QSettings* settings{nullptr};
    const QString keyStyle{"Application/style"};
};

} // namespace xTools
