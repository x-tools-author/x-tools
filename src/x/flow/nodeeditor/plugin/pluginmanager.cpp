/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "pluginmanager.h"

#include <QDir>
#include <QJsonDocument>
#include <QJsonParseError>
#include <QPluginLoader>

#include "plugins/cplugin.h"
#include "plugins/jsplugin.h"
#include "plugins/luaplugin.h"
#include "plugins/pyplugin.h"
#include "plugins/qtplugin.h"

PluginManager::PluginManager(QObject *parent)
    : QObject(parent)
{}

PluginManager::~PluginManager() {}

PluginManager &PluginManager::singleton()
{
    static PluginManager instance;
    return instance;
}

void PluginManager::loadPlugins(const QString &path)
{
    QDir dir(path);
    if (!dir.exists()) {
        qWarning() << "Plugin directory does not exist:" << path;
        return;
    }

    QStringList entryList = dir.entryList(QDir::Files | QDir::NoDotAndDotDot);
    for (QString &entry : entryList) {
        QDir pluginDir(dir.absoluteFilePath(entry));
        loadPlugin(pluginDir.absolutePath());
    }
}

QList<PluginManager::PluginContext> PluginManager::plugins() const
{
    return m_plugins;
}

AbstractPlugin *PluginManager::newPlugin(const QString &libFile)
{
    if (libFile.endsWith(QString(".dll")) || libFile.endsWith(".so")) {
        if (QtPlugin::testLib(libFile)) {
            return new QtPlugin(libFile);
        } else if (CPlugin::testLib(libFile)) {
            return new CPlugin(libFile);
        }
    } else if (libFile.endsWith(".mjs")) {
        if (JsPlugin::testLib(libFile)) {
            return new JsPlugin(libFile);
        }
    } else if (libFile.endsWith(".lua")) {
        if (LuaPlugin::testLib(libFile)) {
            return new LuaPlugin(libFile);
        }
    } else if (libFile.endsWith(".py")) {
        if (PyPlugin::testLib(libFile)) {
            return new PyPlugin(libFile);
        }
    } else {
        qWarning() << "Unsupported plugin type:" << libFile;
    }

    return nullptr;
}

bool PluginManager::runInThread(const QString &libFile) const
{
    if (libFile.endsWith(".py")) {
        return false;
    }

    return true;
}

void PluginManager::loadPlugin(const QString &file)
{
    auto *tmp = newPlugin(file);
    if (tmp) {
        QString caption = tmp->caption();
        QString name = tmp->name();
        PluginContext pluginCtx;
        pluginCtx.libFile = file;
        pluginCtx.caption = caption;
        pluginCtx.name = name;
        m_plugins.append(pluginCtx);
        tmp->deleteLater();
    }
}
