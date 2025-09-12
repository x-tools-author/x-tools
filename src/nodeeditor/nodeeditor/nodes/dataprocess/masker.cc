/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "masker.h"
#include "maskerui.h"

Masker::Masker(QObject *parent)
    : BaseNode(parent)
{}

Masker::~Masker() {}

QString Masker::caption() const
{
    return tr("Masker");
}

QString Masker::name() const
{
    return QString("Masker");
}

unsigned int Masker::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *Masker::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new MaskerUi(this);
    }

    return m_embeddedWidget;
}

void Masker::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    QByteArray bytes = nodeData->bytes();
    if (!bytes.isEmpty()) {
        QByteArray cookedBytes;
        QJsonObject parameters = save();
        quint8 mask = static_cast<quint8>(parameters.value(MaskerParametersKeys().mask).toInt());
        for (int i = 0; i < bytes.length(); i++) {
            quint8 value = quint8(bytes.at(i));
            value ^= mask;
            cookedBytes.append(reinterpret_cast<char *>(&value), 1);
        }

        setOutData(std::make_shared<BaseNodeData>(cookedBytes));
        emit dataUpdated(0);
    }
}