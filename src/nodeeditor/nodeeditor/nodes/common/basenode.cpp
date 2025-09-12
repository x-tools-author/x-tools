/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "basenode.h"
#include "basenodeui.h"

BaseNode::BaseNode(QObject *parent)
    : QtNodes::NodeDelegateModel()
    , m_outData(std::make_shared<BaseNodeData>())
{}

BaseNode::~BaseNode() {}

void BaseNode::setMessage(const QString &message)
{
    if (embeddedWidget()) {
        BaseNodeUi *baseNodeUi = qobject_cast<BaseNodeUi *>(embeddedWidget());
        if (baseNodeUi) {
            baseNodeUi->setMessage(message);
        }
    }
}

QtNodes::NodeDataType BaseNode::dataType(QtNodes::PortType portType,
                                         QtNodes::PortIndex portIndex) const
{
    if (portType == QtNodes::PortType::In) {
        return QtNodes::NodeDataType{QString("QByteArray"), QString("[In]")};
    } else if (portType == QtNodes::PortType::Out) {
        return QtNodes::NodeDataType{QString("QByteArray"), QString("[Out]")};
    }

    return QtNodes::NodeDataType{QString("-"), QString("[-]")};
}

void BaseNode::setInData(std::shared_ptr<QtNodes::NodeData> nodeData,
                         QtNodes::PortIndex const portIndex)
{
    std::shared_ptr<BaseNodeData> cookedNodeData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!cookedNodeData) {
        return;
    }

    if (cookedNodeData->bytes().isEmpty()) {
        return;
    }

    QJsonObject parameters = save();
    BaseNodeParameters data = jsonObject2BaseNodeParameters(parameters);
    if (data.paused) {
        return;
    }

    if (data.fastForward) {
        setOutData(cookedNodeData);
        emit dataUpdated(portIndex);
        return;
    }

    if (m_handleInDataInThread.load()) {
        emit input2thread(nodeData, portIndex);
        return;
    } else {
        handleData(cookedNodeData, portIndex);
    }
}

std::shared_ptr<QtNodes::NodeData> BaseNode::outData(QtNodes::PortIndex const port)
{
    Q_UNUSED(port);
    return outputData();
}

QWidget *BaseNode::embeddedWidget()
{
    return nullptr;
}

bool BaseNode::resizable() const
{
    return false;
}

QJsonObject BaseNode::save() const
{
    m_parametersMutex.lock();
    QJsonObject obj = QtNodes::NodeDelegateModel::save();
    QWidget *w = const_cast<BaseNode *>(this)->embeddedWidget();
    BaseNodeUi *baseNodeUi = qobject_cast<BaseNodeUi *>(w);
    if (baseNodeUi) {
        QJsonObject uiParameters = baseNodeUi->save();
        QStringList keys = uiParameters.keys();
        for (QString &key : keys) {
            obj[key] = uiParameters[key];
        }
    }
    m_parametersMutex.unlock();

    return obj;
}

void BaseNode::load(QJsonObject const &parameters)
{
    m_parametersMutex.lock();
    QtNodes::NodeDelegateModel::load(parameters);
    BaseNodeUi *baseNodeUi = qobject_cast<BaseNodeUi *>(embeddedWidget());
    if (baseNodeUi) {
        baseNodeUi->load(parameters);
    }
    m_parametersMutex.unlock();
}

std::shared_ptr<BaseNodeData> BaseNode::outputData() const
{
    m_outDataMutex.lock();
    std::shared_ptr<BaseNodeData> outData = m_outData;
    m_outDataMutex.unlock();

    return outData;
}

void BaseNode::setOutData(std::shared_ptr<BaseNodeData> nodeData)
{
    m_outDataMutex.lock();
    m_outData.reset();
    m_outData = std::move(nodeData);
    m_outDataMutex.unlock();
}

void BaseNode::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    setOutData(nodeData);
    emit dataUpdated(index);
}
