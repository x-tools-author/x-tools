/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariantMap>
#include <QWidget>

#include "../../xIO.h"
#include "../AbstractIOUi.h"

class Communication;
class CommunicationUi : public AbstractIOUi
{
    Q_OBJECT
public:
    CommunicationUi(xIO::DeviceType type, QWidget *parent = nullptr);

    xIO::DeviceType type() const;

    virtual void setupDevice(Communication *device) { Q_UNUSED(device); };
    virtual void setUiEnabled(bool enabled) { setEnabled(enabled); }

private:
    xIO::DeviceType const m_type;
};
