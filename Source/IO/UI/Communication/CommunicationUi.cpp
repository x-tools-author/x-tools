/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "CommunicationUi.h"

namespace xTools {

CommunicationUi::CommunicationUi(xIO::CommunicationType type, QWidget *parent)
    : AbstractIOUi(parent)
    , m_type(type)
{}

xIO::CommunicationType CommunicationUi::type() const
{
    return m_type;
}

} // namespace xTools
