/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "filter.h"
#include "filterui.h"

#include "common/common.h"

Filter::Filter(QObject *parent)
    : BaseNode(parent)
{
    int option = static_cast<int>(ResponseOption::InputEqualReference);
    int format = static_cast<int>(TextFormat::Ascii);
    int escape = static_cast<int>(EscapeCharacter::None);
    QString reference{"Hello"};

    QJsonObject data;
    FilterParametersKeys keys;
    data[keys.option] = option;
    data[keys.format] = format;
    data[keys.escape] = escape;
    data[keys.reference] = reference;
    load(data);
}

QString Filter::caption() const
{
    return tr("Filter");
}

QString Filter::name() const
{
    return QString("Filter");
}

unsigned int Filter::nPorts(QtNodes::PortType portType) const
{
    if (portType == QtNodes::PortType::In) {
        return 1;
    } else if (portType == QtNodes::PortType::Out) {
        return 1;
    } else {
        return 0;
    }
}

QWidget *Filter::embeddedWidget()
{
    if (!m_embeddedWidget) {
        m_embeddedWidget = new FilterUi(this);
    }
    return m_embeddedWidget;
}

void Filter::handleData(std::shared_ptr<BaseNodeData> nodeData, QtNodes::PortIndex const index)
{
    QByteArray bytes = nodeData->bytes();
    if (bytes.isEmpty()) {
        return;
    }

    QJsonObject parameters = save();
    FilterParametersKeys keys;
    int option = parameters.value(keys.option).toInt();
    int format = parameters.value(keys.format).toInt();
    int escape = parameters.value(keys.escape).toInt();
    QString reference = parameters.value(keys.reference).toString();

    reference = cookedEscapeCharacter(reference, escape);
    QByteArray referenceBytes = string2bytes(reference, format);
    QByteArray ret = responseData(bytes, option, referenceBytes);
    if (ret.isEmpty()) {
        return;
    }

    setOutData(std::move(nodeData));
    emit dataUpdated(index);
}
