/******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKDataStructure.h"
#include "SAKAffixesComboBox.h"

SAKAffixesComboBox::SAKAffixesComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem(tr("None"), SAKDataStructure::AffixesNone);
    addItem("\\r", SAKDataStructure::AffixesR);
    addItem("\\n", SAKDataStructure::AffixesN);
    addItem("\\r\\n", SAKDataStructure::AffixesRN);
    addItem("\\n\\r", SAKDataStructure::AffixesNR);
}
