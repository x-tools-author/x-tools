/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

struct PluginNodeParameterKeys
{
    const QString libFile{"libFile"};
};

class BaseNodeUi;
class AbstractPlugin;
class PluginNode : public BaseNode
{
    Q_OBJECT
public:
    PluginNode(const QString &libFile, QObject *parent = nullptr);
    ~PluginNode() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex const portIndex) override;
    QWidget *embeddedWidget() override;
    QJsonObject save() const override;
    void load(QJsonObject const &parameters) override;

protected:
    void handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index) override;
    void onOutput(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const index);

private:
    QMap<int, std::shared_ptr<BaseNodeData>> m_outDataMap;
    AbstractPlugin *m_plugin;
    QString m_libFile;
    BaseNodeUi *m_embeddedWidget{nullptr};
};
