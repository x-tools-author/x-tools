/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakescapecharactercombobox.h"

#include "sakdatastructure.h"

SAKEscapeCharacterComboBox::SAKEscapeCharacterComboBox(QWidget* parent)
    : SAKComboBox(parent)
{
    addItem(tr("None"), SAKDataStructure::EscapeCharacterOptionNone);
    addItem("\\r", SAKDataStructure::EscapeCharacterOptionR);
    addItem("\\n", SAKDataStructure::EscapeCharacterOptionN);
    addItem("\\r\\n", SAKDataStructure::EscapeCharacterOptionRN);
    addItem("\\n\\r", SAKDataStructure::EscapeCharacterOptionNR);
    addItem("\\r + \\n", SAKDataStructure::EscapeCharacterOptionRAndN);
}
