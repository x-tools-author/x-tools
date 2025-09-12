/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "cplugin.h"

CPlugin::CPlugin(const QString &libFile, QObject *parent)
    : AbstractPlugin(libFile, parent)
{
    m_library = new QLibrary(libFile, this);
}

CPlugin::~CPlugin()
{
    if (isRunning()) {
        quit();
        wait();
    }
}

bool CPlugin::testLib(const QString &file)
{
    CPluginApiV1 apis;
    auto apiVersion = QLibrary::resolve(file, apis.apiVersion.toLatin1().constData());
    auto caption = QLibrary::resolve(file, apis.pluginCaption.toLatin1().constData());
    auto name = QLibrary::resolve(file, apis.pluginName.toLatin1().constData());
    auto inPorts = QLibrary::resolve(file, apis.pluginInPorts.toLatin1().constData());
    auto outPorts = QLibrary::resolve(file, apis.pluginOutPorts.toLatin1().constData());
    auto handle = QLibrary::resolve(file, apis.pluginHandle.toLatin1().constData());

    return apiVersion && caption && name && inPorts && outPorts && handle;
}

QString CPlugin::caption() const
{
    CPluginApiV1 apis;
    const char *api = apis.pluginCaption.toLatin1().constData();
    auto func = (const char *(*) ()) QLibrary::resolve(m_libFile, api);
    if (func) {
        return QString::fromLocal8Bit(func());
    }

    return QString(__FUNCTION__);
}

QString CPlugin::name() const
{
    CPluginApiV1 apis;
    const char *api = apis.pluginName.toLatin1().constData();
    auto func = (const char *(*) ()) QLibrary::resolve(m_libFile, api);
    if (func) {
        return QString::fromLocal8Bit(func());
    }

    return QString(__FUNCTION__);
}

unsigned int CPlugin::inPorts() const
{
    CPluginApiV1 apis;
    const char *api = apis.pluginInPorts.toLatin1().constData();
    auto func = (unsigned int (*)()) QLibrary::resolve(m_libFile, api);
    if (func) {
        return func();
    }

    return 0;
}

unsigned int CPlugin::outPorts() const
{
    CPluginApiV1 apis;
    const char *api = apis.pluginOutPorts.toLatin1().constData();
    auto func = (unsigned int (*)()) QLibrary::resolve(m_libFile, api);
    if (func) {
        return func();
    }

    return 0;
}

QByteArray CPlugin::handleData(QByteArray const &data, int const index)
{
    CPluginApiV1 apis;
    const char *api = apis.pluginHandle.toLatin1().constData();
    auto func = (int (*)(const char *input_data,
                         int input_data_length,
                         unsigned char *output_data,
                         int output_data_length)) QLibrary::resolve(m_libFile, api);
    if (func) {
        char output[1024];
        int len = func(data.constData(), data.size(), (unsigned char *) output, sizeof(output));
        QByteArray result(output, len);
        if (!result.isEmpty()) {
            return result;
        }
    }

    return QByteArray();
}
