/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "abstractplugin.h"

struct LuaPluginApiV1
{
    const QString apiVersion{"X_flow_plugin_get_api_version"};
    const QString pluginCaption{"X_flow_plugin_get_caption"};
    const QString pluginName{"X_flow_plugin_get_name"};
    const QString pluginInPorts{"X_flow_plugin_get_in_ports"};
    const QString pluginOutPorts{"X_flow_plugin_get_out_ports"};
    const QString pluginHandle{"X_flow_plugin_handle_data"};
};

class LuaPlugin : public AbstractPlugin
{
    Q_OBJECT
public:
    explicit LuaPlugin(const QString &libFile, QObject *parent = nullptr);
    ~LuaPlugin() override;
    LuaPlugin(const LuaPlugin &) = delete;
    LuaPlugin &operator=(const LuaPlugin &) = delete;

    static bool testLib(const QString &libFile);

    QString caption() const override;
    QString name() const override;
    unsigned int inPorts() const override;
    unsigned int outPorts() const override;

    QByteArray handleData(QByteArray const &data, int const index) override;
};
