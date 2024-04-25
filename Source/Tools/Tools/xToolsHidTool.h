/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsCommunicationTool.h"

class xToolsHidTool : public xToolsCommunicationTool
{
    Q_OBJECT
public:
    explicit xToolsHidTool(QObject *parent = nullptr);
    ~xToolsHidTool() override;

protected:
    virtual bool initialize(QString &errStr) override;
    virtual void writeBytes(const QByteArray &bytes) override;
    virtual void uninitialize() override;
};