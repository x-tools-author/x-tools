/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "communicationui.h"

#include <QHBoxLayout>

#include "communication.h"
#include "device/devicefactory.h"
#include "device/deviceui.h"

CommunicationUi::CommunicationUi(int type, BaseNode *node, QWidget *parent)
    : BaseNodeUi(node, parent)
    , m_deviceUi(nullptr)
{
    m_deviceUi = DeviceFactory::singleton().newDeviceUi(type);
    setEmbeddedWidget(m_deviceUi);
}

CommunicationUi::~CommunicationUi() {}

DeviceUi *CommunicationUi::deviceUi() const
{
    return m_deviceUi;
}

void CommunicationUi::rebootDevice()
{
    if (m_deviceUi && m_deviceUi->device()) {
        m_deviceUi->closeDevice();
        m_deviceUi->openDevice();
    }
}

QJsonObject CommunicationUi::save() const
{
    QJsonObject parameters = BaseNodeUi::save();
    QJsonObject deviceParameters = m_deviceUi->save();
    QStringList keys = deviceParameters.keys();
    for (QString &key : keys) {
        parameters.insert(key, deviceParameters.value(key));
    }
    return parameters;
}

void CommunicationUi::load(const QJsonObject &parameters)
{
    BaseNodeUi::load(parameters);
    m_deviceUi->load(parameters);
}
