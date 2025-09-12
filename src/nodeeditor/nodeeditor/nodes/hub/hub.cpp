/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "hub.h"

Hub::Hub(int inPorts, int outPorts, QObject *parent)
    : BaseNode(parent)
    , m_inPorts(inPorts)
    , m_outPorts(outPorts)
    , m_nodeData(std::make_shared<BaseNodeData>())
{}

Hub::~Hub() {}

QString Hub::caption() const
{
    return tr("%1in%2out").arg(m_inPorts).arg(m_outPorts);
}

QString Hub::name() const
{
    return QString("%1in%2out").arg(m_inPorts).arg(m_outPorts);
}

unsigned int Hub::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return m_inPorts;
    } else if (portType == QtNodes::PortType::Out) {
        return m_outPorts;
    } else {
        return 0;
    }
}

void Hub::setInData(std::shared_ptr<QtNodes::NodeData> nodeData, QtNodes::PortIndex const portIndex)
{
    std::shared_ptr<BaseNodeData> cookedNodeData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
    if (!cookedNodeData) {
        return;
    }

    if (cookedNodeData->bytes().isEmpty()) {
        return;
    }

    for (int i = 0; i < m_outPorts; ++i) {
        m_nodeData.reset();
        m_nodeData = std::dynamic_pointer_cast<BaseNodeData>(nodeData);
        emit dataUpdated(i);
    }
}

std::shared_ptr<QtNodes::NodeData> Hub::outData(QtNodes::PortIndex const port)
{
    return m_nodeData;
}

QWidget *Hub::embeddedWidget()
{
    return nullptr;
}
