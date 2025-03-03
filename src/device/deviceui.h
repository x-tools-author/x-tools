/***************************************************************************************************
 * Copyright 2024-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of eTools project.
 *
 * eTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QVariantMap>
#include <QWidget>

#include "common/xtools.h"
#include "page/common/abstractioui.h"

class Communication;
class CommunicationUi : public AbstractIOUi
{
    Q_OBJECT
public:
    CommunicationUi(CommunicationType type, QWidget *parent = nullptr);

    CommunicationType type() const;
    virtual QList<QWidget *> communicationControllers();

private:
    CommunicationType const m_type;
};
