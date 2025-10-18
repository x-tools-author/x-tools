/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "pluginnode.h"

#include "nodeeditor/nodes/common/basenodedata.h"
#include "nodeeditor/nodes/common/basenodeui.h"

#include "pluginmanager.h"
#include "plugins/abstractplugin.h"

#include <QTimer>

PluginNode::PluginNode(const QString &libFile, QObject *parent)
    : BaseNode(parent)
    , m_plugin(nullptr)
{
    m_plugin = PluginManager::singleton().newPlugin(libFile);
    m_handleInDataInThread.store(PluginManager::singleton().runInThread(libFile));
    if (!m_plugin) {
        return;
    }

    for (int i = 0; i < m_plugin->outPorts(); ++i) {
        m_outDataMap.insert(i, nullptr);
    }

    connect(this, &PluginNode::input2thread, m_plugin, &AbstractPlugin::inputBytes);
    connect(m_plugin, &AbstractPlugin::outputBytes, this, &PluginNode::onOutput);
    connect(m_plugin, &AbstractPlugin::finished, this, [=]() {
        QTimer::singleShot(1000, this, [=]() { m_plugin->start(); });
    });

    if (m_handleInDataInThread.load()) {
        m_plugin->start();
    }
}

PluginNode::~PluginNode()
{
    if (m_plugin) {
        m_plugin->deleteLater();
        m_plugin = nullptr;
    }
}

QString PluginNode::caption() const
{
    if (m_plugin) {
        return m_plugin->caption();
    }

    return QString(__FUNCTION__);
}

QString PluginNode::name() const
{
    if (m_plugin) {
        return m_plugin->name();
    }

    return QString("Unknown Name");
}

unsigned int PluginNode::nPorts(QtNodes::PortType portType) const
{
    if (m_plugin == nullptr) {
        return 0;
    }

    if (portType == QtNodes::PortType::In) {
        return m_plugin->inPorts();
    } else if (portType == QtNodes::PortType::Out) {
        return m_plugin->outPorts();
    } else {
        return 0;
    }
}

std::shared_ptr<QtNodes::NodeData> PluginNode::outData(QtNodes::PortIndex const portIndex)
{
    if (portIndex >= 0 && portIndex < m_outDataMap.size()) {
        return m_outDataMap[portIndex];
    }

    return nullptr;
}

QWidget *PluginNode::embeddedWidget()
{
    if (m_plugin) {
        if (m_embeddedWidget == nullptr) {
            QWidget *w = m_plugin->embeddedWidget();
            if (w) {
                m_embeddedWidget = new BaseNodeUi(this);
                m_embeddedWidget->setEmbeddedWidget(w);
                m_embeddedWidget->adjustSize();
            }
        }
    }

    return m_embeddedWidget;
}

QJsonObject PluginNode::save() const
{
    QJsonObject parameters = BaseNode::save();
    PluginNodeParameterKeys keys;
    parameters.insert(keys.libFile, m_libFile);
    if (m_plugin) {
        QJsonObject pluginParameters = m_plugin->save();
        parameters.insert("plugin", pluginParameters);
    }
    return parameters;
}

void PluginNode::load(QJsonObject const &parameters)
{
    BaseNode::load(parameters);
    PluginNodeParameterKeys keys;
    m_libFile = parameters.value(keys.libFile).toString();
    if (m_plugin) {
        m_plugin->load(parameters.value("plugin").toObject());
    }
}

void PluginNode::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    if (!m_plugin) {
        return;
    }

    QByteArray bytes = m_plugin->handleData(nodeData->bytes(), index);
    if (index >= 0 && index < m_outDataMap.size() && !bytes.isEmpty()) {
        if (m_outDataMap[index]) {
            m_outDataMap[index].reset();
        }

        m_outDataMap[index] = std::make_shared<BaseNodeData>(bytes);
        emit dataUpdated(index);
    }
}

void PluginNode::onOutput(std::shared_ptr<QtNodes::NodeData> nodeData,
                          QtNodes::PortIndex const index)
{
    auto cookedData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!cookedData) {
        return;
    }

    if (index >= 0 && index < m_outDataMap.size()) {
        if (m_outDataMap[index]) {
            m_outDataMap[index].reset();
        }

        m_outDataMap[index] = cookedData;
        emit this->dataUpdated(index);
    }
}