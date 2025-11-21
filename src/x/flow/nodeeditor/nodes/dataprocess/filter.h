/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "nodeeditor/nodes/common/basenode.h"

struct FilterParametersKeys
{
    QString option{"option"};
    QString format{"format"};
    QString escape{"escape"};
    QString reference{"reference"};
};

class Filter : public BaseNode
{
    Q_OBJECT
public:
    Filter(QObject *parent = nullptr);

    QString caption() const override;
    QString name() const override;
    unsigned int nPorts(QtNodes::PortType portType) const override;
    QWidget *embeddedWidget() override;

protected:
    void handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index) override;

private:
    QWidget *m_embeddedWidget{nullptr};
};
