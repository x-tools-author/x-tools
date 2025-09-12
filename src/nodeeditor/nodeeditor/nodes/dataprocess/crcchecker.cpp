/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "crcchecker.h"
#include "crccheckerui.h"

#include "common/crc.h"
#include "nodeeditor/nodes/common/basenodeui.h"

CrcChecker::CrcChecker(QObject *parent)
    : BaseNode(parent)
{}

CrcChecker::~CrcChecker() {}

QString CrcChecker::caption() const
{
    return tr("CRC Checker");
}

QString CrcChecker::name() const
{
    return QString("CRCChecker");
}

unsigned int CrcChecker::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *CrcChecker::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new CrcCheckerUi(this);
    }
    return m_embeddedWidget;
}

void CrcChecker::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    if (nodeData->bytes().isEmpty()) {
        return;
    }

    QJsonObject parameters = save();
    CrcCheckerParametersKeys keys;
    int startIndex = parameters.value(keys.startIndex).toInt();
    int endIndex = parameters.value(keys.endIndex).toInt();
    int algorithm = parameters.value(keys.algorithm).toInt();
    bool bigEndian = parameters.value(keys.bigEndian).toBool();

    int bitsWidth = CRC::bitsWidth(static_cast<CRC::Algorithm>(algorithm));
    int bytesWidth = bitsWidth / 8;

    CRC::Context ctx;
    ctx.algorithm = static_cast<CRC::Algorithm>(algorithm);
    ctx.bigEndian = bigEndian;
    ctx.startIndex = startIndex;
    ctx.endIndex = endIndex;
    ctx.data = nodeData->bytes().left(nodeData->bytes().length() - bytesWidth); // CRC计算数据

    QByteArray refCrcValue = CRC::calculate(ctx);                      // 参考crc值
    QByteArray actuallyCrcValue = nodeData->bytes().right(bytesWidth); // 实际crc值
    if (actuallyCrcValue != refCrcValue) {
        if (embeddedWidget()) {
            auto *nodeUi = qobject_cast<CrcCheckerUi *>(embeddedWidget());
            if (nodeUi == nullptr) {
                return;
            }

            QString msg = tr("CRC check failed, expected: %1, got: %2")
                              .arg(refCrcValue.toHex().toUpper())
                              .arg(actuallyCrcValue.toHex().toUpper());
            qWarning() << msg;
            nodeUi->setMessage(msg);
        }
    } else {
        if (embeddedWidget()) {
            auto *nodeUi = qobject_cast<CrcCheckerUi *>(embeddedWidget());
            if (nodeUi) {
                nodeUi->setMessage(tr("CRC checking passed."));
            }
        }

        setOutData(nodeData);
        emit dataUpdated(0);
    }
}
