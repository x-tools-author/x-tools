/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>

class PluginNode;
class AbstractPlugin;
class PluginManager : public QObject
{
    Q_OBJECT
private:
    explicit PluginManager(QObject *parent = nullptr);
    PluginManager(const PluginManager &) = delete;
    PluginManager &operator=(const PluginManager &) = delete;

public:
    struct PluginContext
    {
        QString name;
        QString caption;
        QString libFile;
    };

public:
    ~PluginManager() override;
    static PluginManager &singleton();

    void loadPlugins(const QString &path);
    QList<PluginContext> plugins() const;
    AbstractPlugin *newPlugin(const QString &libFile);
    bool runInThread(const QString &libFile) const;

private:
    void loadPlugin(const QString &file);

private:
    QList<PluginContext> m_plugins;
};
