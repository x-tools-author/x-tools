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

class Hub : public BaseNode
{
    Q_OBJECT
public:
    Hub(int inPorts, int outPorts, QObject *parent = nullptr);
    ~Hub() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    void setInData(std::shared_ptr<QtNodes::NodeData> nodeData,
                   QtNodes::PortIndex const portIndex) override;
    std::shared_ptr<QtNodes::NodeData> outData(QtNodes::PortIndex const port) override;
    QWidget *embeddedWidget() override;

private:
    int m_inPorts;
    int m_outPorts;
    std::shared_ptr<BaseNodeData> m_nodeData;
};
