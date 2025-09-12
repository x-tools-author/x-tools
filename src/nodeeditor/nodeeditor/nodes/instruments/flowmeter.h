/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

class Flowmeter : public BaseNode
{
    Q_OBJECT
public:
    Flowmeter(QObject *parent = nullptr);
    ~Flowmeter() override;

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

protected:
    void handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index) override;

private:
    QWidget *m_embeddedWidget{nullptr};
};
