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

namespace xTools {

class HdpiManagerPrivate : public QObjectPrivate
{
    Q_DECLARE_PUBLIC(HdpiManager)
public:
    HdpiManagerPrivate() {}
    ~HdpiManagerPrivate() {}

    void initHdpiMenu()
    {
        if (hdpiMenu) {
            return;
        }
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        Q_Q(HdpiManager);
        hdpiMenu = new QMenu();
        QActionGroup* actionGroup = new QActionGroup(hdpiMenu);
        typedef Qt::HighDpiScaleFactorRoundingPolicy Policy;
        QMap<Policy, QString> policyMap;
        policyMap.insert(Policy::Unset, QObject::tr("System"));
        policyMap.insert(Policy::Round, QObject::tr("Round up for .5 and above"));
        policyMap.insert(Policy::Ceil, QObject::tr("Always round up"));
        policyMap.insert(Policy::Floor, QObject::tr("Always round down"));
        policyMap.insert(Policy::RoundPreferFloor, QObject::tr("Round up for .75 and above"));
        policyMap.insert(Policy::PassThrough, QObject::tr("Don't round"));

        for (auto it = policyMap.begin(); it != policyMap.end(); ++it) {
            QString name = it.value();
            int policy = static_cast<int>(it.key());
            auto action = hdpiMenu->addAction(name, hdpiMenu, [=]() {
                if (!settings) {
                    return;
                }

                this->settings->setValue(keyHdpi, policy);
                emit q->hdpiChanged();
            });

            actionGroup->addAction(action);
            action->setCheckable(true);
            if (policy == static_cast<int>(qApp->highDpiScaleFactorRoundingPolicy())) {
                action->setChecked(true);
            }
        }

        hdpiMenu->addActions(actionGroup->actions());
#endif
    }

    void setupHdpi()
    {
#if QT_VERSION >= QT_VERSION_CHECK(5, 14, 0)
        if (!settings) {
            return;
        }

        int defaultPolicy = static_cast<int>(QGuiApplication::highDpiScaleFactorRoundingPolicy());
        int policy = settings->value(keyHdpi, defaultPolicy).toInt();

        const auto cookedPolicy = static_cast<Qt::HighDpiScaleFactorRoundingPolicy>(policy);
        QGuiApplication::setHighDpiScaleFactorRoundingPolicy(cookedPolicy);
        qInfo() << "The current high dpi policy is:" << cookedPolicy;
#endif
    }

public:
    QMenu* hdpiMenu{nullptr};
    QSettings* settings{nullptr};

private:
    const QString keyHdpi{"Application/hdpi"};
};

} // namespace xTools
