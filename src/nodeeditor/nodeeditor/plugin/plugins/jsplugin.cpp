/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "jsplugin.h"

#include <QJSEngine>

JsPlugin::JsPlugin(const QString &libFile, QObject *parent)
    : AbstractPlugin(libFile, parent)
{}

JsPlugin::~JsPlugin()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

bool JsPlugin::testLib(const QString &file)
{
    JsPluginApiV1 apis;
    QJSEngine jsEngine;
    QJSValue js = jsEngine.importModule(file);
    if (!js.isObject()) {
        qWarning() << "Failed to load JS plugin:" << file;
        return false;
    }

    QJSValue apiVersion = js.property(apis.apiVersion);
    QJSValue pluginCaption = js.property(apis.pluginCaption);
    QJSValue pluginName = js.property(apis.pluginName);
    QJSValue pluginInPorts = js.property(apis.pluginInPorts);
    QJSValue pluginOutPorts = js.property(apis.pluginOutPorts);
    QJSValue pluginHandle = js.property(apis.pluginHandle);

    return apiVersion.isCallable() && pluginCaption.isCallable() && pluginName.isCallable()
           && pluginInPorts.isCallable() && pluginOutPorts.isCallable()
           && pluginHandle.isCallable();
}

QString JsPlugin::caption() const
{
    JsPluginApiV1 apis;
    QJSEngine jsEngine;
    QJSValue js = jsEngine.importModule(m_libFile);
    QJSValue pluginCaption = js.property(apis.pluginCaption);
    if (pluginCaption.isCallable()) {
        QJSValue caption = pluginCaption.call();
        if (caption.isString()) {
            return caption.toString();
        }
    }

    return QString(__FUNCTION__);
}

QString JsPlugin::name() const
{
    JsPluginApiV1 apis;
    QJSEngine jsEngine;
    QJSValue js = jsEngine.importModule(m_libFile);
    QJSValue pluginCaption = js.property(apis.pluginName);
    if (pluginCaption.isCallable()) {
        QJSValue name = pluginCaption.call();
        if (name.isString()) {
            return name.toString();
        }
    }

    return QString(__FUNCTION__);
}

unsigned int JsPlugin::inPorts() const
{
    JsPluginApiV1 apis;
    QJSEngine jsEngine;
    QJSValue js = jsEngine.importModule(m_libFile);
    QJSValue pluginCaption = js.property(apis.pluginInPorts);
    if (pluginCaption.isCallable()) {
        QJSValue ports = pluginCaption.call();
        if (ports.isNumber()) {
            return ports.toUInt();
        }
    }

    return 0;
}

unsigned int JsPlugin::outPorts() const
{
    JsPluginApiV1 apis;
    QJSEngine jsEngine;
    QJSValue js = jsEngine.importModule(m_libFile);
    QJSValue pluginCaption = js.property(apis.pluginOutPorts);
    if (pluginCaption.isCallable()) {
        QJSValue ports = pluginCaption.call();
        if (ports.isNumber()) {
            return ports.toUInt();
        }
    }

    return 0;
}

QByteArray JsPlugin::handleData(QByteArray const &data, int const index)
{
    JsPluginApiV1 apis;
    QJSEngine jsEngine;
    QJSValue js = jsEngine.importModule(m_libFile);
    QJSValue pluginCaption = js.property(apis.pluginHandle);
    if (pluginCaption.isCallable()) {
        QString inputData = QString::fromLatin1(data.toHex());
        QJSValueList args;
        args << QJSValue(inputData) << QJSValue(index);
        QJSValue ports = pluginCaption.call(args);
        if (ports.isString()) {
            return QByteArray::fromHex(ports.toString().toLatin1());
        }
    }

    return QByteArray();
}
