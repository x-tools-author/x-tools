/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QLibrary>

#include "abstractplugin.h"

struct CPluginApiV1
{
    const QString apiVersion{"x_flow_plugin_get_api_version"};
    const QString pluginCaption{"x_flow_plugin_get_caption"};
    const QString pluginName{"x_flow_plugin_get_name"};
    const QString pluginInPorts{"x_flow_plugin_get_in_ports"};
    const QString pluginOutPorts{"x_flow_plugin_get_out_ports"};
    const QString pluginHandle{"x_flow_plugin_handle_data"};
};

class CPlugin : public AbstractPlugin
{
    Q_OBJECT
public:
    explicit CPlugin(const QString &libFile, QObject *parent = nullptr);
    ~CPlugin() override;
    CPlugin(const CPlugin &) = delete;
    CPlugin &operator=(const CPlugin &) = delete;

    static bool testLib(const QString &file);

    QString caption() const override;
    QString name() const override;
    unsigned int inPorts() const override;
    unsigned int outPorts() const override;

    QByteArray handleData(QByteArray const &data, int const index) override;

private:
    QLibrary *m_library;
};
