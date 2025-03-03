/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "deviceui.h"

#include "device/device.h"

DeviceUi::DeviceUi(CommunicationType type, QWidget *parent)
    : AbstractIOUi(parent)
    , m_type(type)
{}

CommunicationType DeviceUi::type() const
{
    return m_type;
}

QList<QWidget *> DeviceUi::communicationControllers()
{
    return {};
}
