/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/NodeDelegateModel>

#include <QMutex>

#include "nodeeditor/nodes/common/basenodedata.h"

#define BASE_NODE_PARAMETERS_KEY QString("base-node")

class BaseNode : public QtNodes::NodeDelegateModel
{
    Q_OBJECT
public:
    BaseNode(QObject *parent = nullptr);
    ~BaseNode() override;
    void setMessage(const QString &message);

public:
    struct BaseNodeParameters
    {
        bool expanded;
        bool fastForward;
        bool paused;
    };

    struct BaseNodeParametersKeys
    {
        const QString expanded{"expanded"};
        const QString fastForward{"fastForward"};
        const QString paused{"paused"};
    };

    static BaseNodeParameters jsonObject2BaseNodeParameters(const QJsonObject &parameters)
    {
        BaseNodeParameters data;
        BaseNodeParametersKeys keys;
        QVariantMap parametersMap = parameters.toVariantMap();
        data.expanded = parametersMap.value(keys.expanded, true).toBool();
        data.fastForward = parametersMap.value(keys.fastForward, false).toBool();
        data.paused = parametersMap.value(keys.paused, false).toBool();
        return data;
    }

public:
    // clang-format off
    QtNodes::NodeDataType dataType(QtNodes::PortType portType, QtNodes::PortIndex portIndex) const override;
    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex) override;
    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex const port) override;
    QWidget *embeddedWidget() override;
    bool resizable() const override;
    QJsonObject save() const override;
    void load(QJsonObject const &parameters) override;
    // clang-format on

    std::shared_ptr<BaseNodeData> outputData() const;
    void setOutData(std::shared_ptr<BaseNodeData> nodeData);

signals:
    void input2thread(std::shared_ptr<QtNodes::NodeData> data, QtNodes::PortIndex const index);

protected:
    // 输入数据处理接口
    virtual void handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index);

protected:
    std::atomic_bool m_handleInDataInThread{false};

private:
    std::shared_ptr<BaseNodeData> m_outData;
    mutable QMutex m_outDataMutex;
    mutable QMutex m_parametersMutex; // Locker for parameters in save/load functions
};
