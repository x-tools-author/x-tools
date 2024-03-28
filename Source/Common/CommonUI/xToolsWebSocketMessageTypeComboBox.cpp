/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsWebSocketMessageTypeComboBox.h"

#include "xToolsDataStructure.h"

xToolsWebSocketMessageTypeComboBox::xToolsWebSocketMessageTypeComboBox(QWidget* parent)
    : xToolsComboBox{parent}
{
    auto types = xToolsDataStructure::supportedWebSocketSendingTypes();
    for (auto &type : types) {
        auto name = xToolsDataStructure::webSocketSendingTypeName(type.toInt());
        addItem(name, type.toInt());
    }
}
