/***************************************************************************************************
 * Copyright 2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsHidTool.h"

#include <hidapi.h>

xToolsHidTool::xToolsHidTool(QObject *parent)
    : xToolsCommunicationTool{parent}
{}

xToolsHidTool::~xToolsHidTool() {}

bool xToolsHidTool::initialize(QString &errStr)
{
    return false;
}

void xToolsHidTool::writeBytes(const QByteArray &bytes)
{
    Q_UNUSED(bytes);
}

void xToolsHidTool::uninitialize()
{
    return;
}