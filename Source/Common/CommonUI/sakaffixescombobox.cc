/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakaffixescombobox.h"
#include "sakdatastructure.h"

SAKAffixesComboBox::SAKAffixesComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem(tr("None"), SAKDataStructure::AffixesNone);
    addItem("\\r", SAKDataStructure::AffixesR);
    addItem("\\n", SAKDataStructure::AffixesN);
    addItem("\\r\\n", SAKDataStructure::AffixesRN);
    addItem("\\n\\r", SAKDataStructure::AffixesNR);
}
