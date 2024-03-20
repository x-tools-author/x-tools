/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsAffixesComboBox.h"
#include "xToolsDataStructure.h"

xToolsAffixesComboBox::xToolsAffixesComboBox(QWidget *parent)
    : xToolsComboBox(parent)
{
    addItem(tr("None"), xToolsDataStructure::AffixesNone);
    addItem("\\r", xToolsDataStructure::AffixesR);
    addItem("\\n", xToolsDataStructure::AffixesN);
    addItem("\\r\\n", xToolsDataStructure::AffixesRN);
    addItem("\\n\\r", xToolsDataStructure::AffixesNR);
}
