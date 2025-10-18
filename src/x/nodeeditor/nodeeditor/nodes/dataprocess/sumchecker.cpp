/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sumchecker.h"
#include "sumcheckerui.h"

SumChecker::SumChecker(QObject *parent)
    : BaseNode(parent)
{}

SumChecker::~SumChecker() {}

QString SumChecker::caption() const
{
    return tr("Sum Checker");
}

QString SumChecker::name() const
{
    return QString("SumChecker");
}

unsigned int SumChecker::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *SumChecker::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new SumCheckerUi(this);
    }

    return m_embeddedWidget;
}

void SumChecker::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    if (nodeData->bytes().isEmpty()) {
        return;
    }

    auto parameters = save();
    int resultBytes = parameters.value(SumCheckerParametersKeys().resultBytes).toInt();

    QByteArray bytes = nodeData->bytes();
    quint64 sum = 0;
    for (auto byte : bytes.left(bytes.size() - resultBytes)) {
        sum += byte;
    }

    bool bigEndian = parameters.value(SumCheckerParametersKeys().bigEndian).toBool();

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

    QByteArray ref = bytes.right(resultBytes);
    if (result != ref) {
        qWarning() << "Sum check failed!";
    } else {
        setOutData(std::make_shared<BaseNodeData>(bytes + result));
        emit dataUpdated(0);
    }
}