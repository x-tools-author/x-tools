/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sumcalculator.h"
#include "sumcalculatorui.h"

SumCalculator::SumCalculator(QObject *parent)
    : BaseNode(parent)
{}

SumCalculator::~SumCalculator() {}

QString SumCalculator::caption() const
{
    return tr("Sum Calculator");
}

QString SumCalculator::name() const
{
    return QString("SumCalculator");
}

unsigned int SumCalculator::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *SumCalculator::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new SumCalculatorUi(this);
    }

    return m_embeddedWidget;
}

void SumCalculator::handleData(std::shared_ptr<BaseNodeData> nodeData,
                               QtNodes::PortIndex const index)
{
    if (nodeData->bytes().isEmpty()) {
        return;
    }

    QByteArray bytes = nodeData->bytes();
    quint64 sum = 0;
    for (auto byte : bytes) {
        sum += byte;
    }

    auto parameters = save();
    int resultBytes = parameters.value(SumCalculatorParametersKeys().resultBytes).toInt();
    bool bigEndian = parameters.value(SumCalculatorParametersKeys().bigEndian).toBool();

    QByteArray result;
    if (resultBytes == SumBytes1) {
        result.append(static_cast<char>(sum));
    } else if (resultBytes == SumBytes2) {
        result.append(static_cast<char>(sum >> 8));
        result.append(static_cast<char>(sum));
    } else if (resultBytes == SumBytes4) {
        result.append(static_cast<char>(sum >> 24));
        result.append(static_cast<char>(sum >> 16));
        result.append(static_cast<char>(sum >> 8));
        result.append(static_cast<char>(sum));
    } else {
        result.append(static_cast<char>(sum >> 56));
        result.append(static_cast<char>(sum >> 48));
        result.append(static_cast<char>(sum >> 40));
        result.append(static_cast<char>(sum >> 32));
        result.append(static_cast<char>(sum >> 24));
        result.append(static_cast<char>(sum >> 16));
        result.append(static_cast<char>(sum >> 8));
        result.append(static_cast<char>(sum));
    }

    if (!bigEndian) {
        std::reverse(result.begin(), result.end());
    }

    setOutData(std::make_shared<BaseNodeData>(bytes + result));
    emit dataUpdated(0);
}
