/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "crccalculator.h"
#include "crccalculatorui.h"

#include "common/crc.h"

CrcCalculator::CrcCalculator(QObject *parent)
    : BaseNode(parent)
{}

CrcCalculator::~CrcCalculator() {}

QString CrcCalculator::caption() const
{
    return tr("CRC Calculator");
}

QString CrcCalculator::name() const
{
    return QString("CrcCalculator");
}

unsigned int CrcCalculator::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *CrcCalculator::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new CrcCalculatorUi(this);
    }
    return m_embeddedWidget;
}

void CrcCalculator::handleData(std::shared_ptr<BaseNodeData> nodeData,
                               QtNodes::PortIndex const index)
{
    if (nodeData->bytes().isEmpty()) {
        return;
    }

    QJsonObject parameters = save();
    CrcCalculatorParametersKeys keys;
    int startIndex = parameters.value(keys.startIndex).toInt();
    int endIndex = parameters.value(keys.endIndex).toInt();
    int algorithm = parameters.value(keys.algorithm).toInt();
    bool bigEndian = parameters.value(keys.bigEndian).toBool();

    CRC::Context ctx;
    ctx.algorithm = static_cast<CRC::Algorithm>(algorithm);
    ctx.bigEndian = bigEndian;
    ctx.startIndex = startIndex;
    ctx.endIndex = endIndex;
    ctx.data = nodeData->bytes();

    QByteArray ret = CRC::calculate(ctx);
    setOutData(std::make_shared<BaseNodeData>(ctx.data + ret));
    emit dataUpdated(index);
}
