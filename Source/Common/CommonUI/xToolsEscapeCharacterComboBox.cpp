/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsEscapeCharacterComboBox.h"

#include "xToolsDataStructure.h"

xToolsEscapeCharacterComboBox::xToolsEscapeCharacterComboBox(QWidget* parent)
    : xToolsComboBox(parent)
{
    addItem(tr("None"), xToolsDataStructure::EscapeCharacterNone);
    addItem("\\r", xToolsDataStructure::EscapeCharacterR);
    addItem("\\n", xToolsDataStructure::EscapeCharacterN);
    addItem("\\r\\n", xToolsDataStructure::EscapeCharacterRN);
    addItem("\\n\\r", xToolsDataStructure::EscapeCharacterNR);
    addItem("\\r + \\n", xToolsDataStructure::EscapeCharacterRAndN);
}
