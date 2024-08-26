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

namespace xTools {

class Communication;
class CommunicationUi : public AbstractIOUi
{
    Q_OBJECT
public:
    CommunicationUi(xIO::CommunicationType type, QWidget *parent = nullptr);

    xIO::CommunicationType type() const;

    virtual void setUiEnabled(bool enabled);

private:
    xIO::CommunicationType const m_type;
};

} // namespace xTools
