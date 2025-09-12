/***************************************************************************************************
 * Copyright 2023-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "debugoutput.h"
#include "debugoutputui.h"

DebugOutput::DebugOutput(QObject *parent)
    : BaseNode(parent)
{}

DebugOutput::~DebugOutput() {}

QString DebugOutput::caption() const
{
    return tr("Debug Output");
}

QString DebugOutput::name() const
{
    return QString("DebugOutput");
}

unsigned int DebugOutput::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 0;
    } else {
        return 0;
    }
}

QWidget *DebugOutput::embeddedWidget()
{
    if (m_embeddedWidget == nullptr) {
        m_embeddedWidget = new DebugOutputUi(this);
    }

    return m_embeddedWidget;
}
