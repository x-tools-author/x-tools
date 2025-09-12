/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "qtplugin.h"

#include <QPluginLoader>

QtPlugin::QtPlugin(const QString &libFile, QObject *parent)
    : AbstractPlugin(libFile, parent)
    , m_plugin(nullptr)
{
    QPluginLoader loader(libFile, this);
    m_plugin = qobject_cast<XFlowPluginInterfaceV1 *>(loader.instance());
    m_plugin->setLanguage(m_language);
}

QtPlugin::~QtPlugin()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

bool QtPlugin::testLib(const QString &file)
{
    QPluginLoader loader(file);
    QObject *obj = loader.instance();
    if (!obj) {
        return false;
    }

    XFlowPluginInterfaceV1 *plugin = qobject_cast<XFlowPluginInterfaceV1 *>(obj);
    if (!plugin) {
        return false;
    }

    return true;
}

QString QtPlugin::caption() const
{
    if (m_plugin) {
        return m_plugin->caption();
    }

    return QString(__FUNCTION__);
}

QString QtPlugin::name() const
{
    if (m_plugin) {
        return m_plugin->name();
    }

    return QString(__FUNCTION__);
}

unsigned int QtPlugin::inPorts() const
{
    if (m_plugin) {
        return m_plugin->inPorts();
    }

    return 0;
}

unsigned int QtPlugin::outPorts() const
{
    if (m_plugin) {
        return m_plugin->outPorts();
    }

    return 0;
}

QWidget *QtPlugin::embeddedWidget()
{
    if (m_plugin && m_embeddedWidget == nullptr) {
        m_embeddedWidget = m_plugin->newWidget();
    }

    return m_embeddedWidget;
}

QByteArray QtPlugin::handleData(QByteArray const &data, int const index)
{
    if (m_plugin) {
        return m_plugin->handleData(data, index, m_embeddedWidget);
    }

    return QByteArray();
}

QJsonObject QtPlugin::save() const
{
    if (m_plugin) {
        return m_plugin->save(m_embeddedWidget);
    }

    return QJsonObject();
}

void QtPlugin::load(const QJsonObject &parameters)
{
    if (m_plugin) {
        m_plugin->load(parameters, m_embeddedWidget);
    }
}