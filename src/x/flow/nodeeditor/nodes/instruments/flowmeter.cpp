/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "flowmeter.h"
#include "flowmeterui.h"

#include <QThread>

Flowmeter::Flowmeter(QObject *parent)
    : BaseNode(parent)
{}

Flowmeter::~Flowmeter() {}

QString Flowmeter::caption() const
{
    return tr("Flowmeter");
}

QString Flowmeter::name() const
{
    return QString("Flowmeter");
}

unsigned int Flowmeter::nPorts(QtNodes::PortType portType) const
{
    return 1;
}

void Flowmeter::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    FlowmeterUi *ui = qobject_cast<FlowmeterUi *>(embeddedWidget());
    if (ui) {
        ui->addBytes(nodeData->bytes().size());
    }

    setOutData(nodeData);
    emit dataUpdated(index);
}

QWidget *Flowmeter::embeddedWidget()
{
    if (QThread::currentThread() != thread()) {
        return m_embeddedWidget;
    }

    if (!m_embeddedWidget) {
        m_embeddedWidget = new FlowmeterUi(this);
    }

    return m_embeddedWidget;
}
