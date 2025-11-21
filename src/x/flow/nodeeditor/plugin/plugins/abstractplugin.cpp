/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "abstractplugin.h"

#include "nodeeditor/nodes/common/basenodedata.h"

QJsonObject abstractPluginParametersToJsonObject(const AbstractPluginParameters &parameters)
{
    AbstractPluginParametersKeys keys;
    QJsonObject jsonObj;
    jsonObj.insert(keys.pluginId, parameters.pluginId);
    jsonObj.insert(keys.pluginName, parameters.pluginId);
    return jsonObj;
}

AbstractPluginParameters abstractPluginParametersFromJsonObject(const QJsonObject &jsonObj)
{
    AbstractPluginParameters parameters;
    AbstractPluginParametersKeys keys;
    parameters.pluginId = jsonObj.value(keys.pluginId).toString();
    parameters.pluginName = jsonObj.value(keys.pluginName).toString();
    return parameters;
}

QString AbstractPlugin::m_language = "en";
AbstractPlugin::AbstractPlugin(const QString &libFile, QObject *parent)
    : QThread(parent)
    , m_libFile(libFile)
{}

AbstractPlugin::~AbstractPlugin()
{
    if (isRunning()) {
        exit();
        wait();
    }
}

void AbstractPlugin::setLanguage(const QString &language)
{
    m_language = language;
}

void AbstractPlugin::inputBytes(std::shared_ptr<QtNodes::NodeData> nodeData,
                                QtNodes::PortIndex const index)
{
    if (!nodeData) {
        return;
    }

    emit input2thread(nodeData, index);
}

QString AbstractPlugin::caption() const
{
    return QString("Unknown Caption");
}

QString AbstractPlugin::name() const
{
    return QString("Unknown Name");
}

unsigned int AbstractPlugin::inPorts() const
{
    return 0;
}

unsigned int AbstractPlugin::outPorts() const
{
    return 0;
}

QWidget *AbstractPlugin::embeddedWidget()
{
    return nullptr;
}

QByteArray AbstractPlugin::handleData(QByteArray const &data, int const index)
{
    Q_UNUSED(index);
    return data;
}

QJsonObject AbstractPlugin::save() const
{
    return QJsonObject();
}

void AbstractPlugin::load(const QJsonObject &parameters)
{
    Q_UNUSED(parameters);
}

void AbstractPlugin::run()
{
    QObject *obj = new QObject();
    connect(this,
            &AbstractPlugin::input2thread,
            obj,
            [=](std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const index) {
                if (!nodeData) {
                    return;
                }

                auto cookedData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
                if (!cookedData) {
                    return;
                }

                QByteArray result = handleData(cookedData->bytes(), index);
                if (!result.isEmpty()) {
                    emit outputBytes(std::make_shared<BaseNodeData>(result), index);
                }
            });

    exec();
    obj->deleteLater();
    obj = nullptr;
}
