/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKDataStructure.hh"
#include "SAKResponseOptionComboBox.hh"

SAKResponseOptionComboBox::SAKResponseOptionComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
    addItem(tr("Disable"), SAKDataStructure::ResponseOptionDisable);
    addItem(tr("Echo"), SAKDataStructure::ResponseOptionEcho);
    addItem(tr("Aways"), SAKDataStructure::ResponseOptionAways);
    addItem(tr("InputEqualReference"), SAKDataStructure::ResponseOptionInputEqualReference);
    addItem(tr("InputContainReference"), SAKDataStructure::ResponseOptionInputContainReference);
    addItem(tr("InputDiscontainReference"), SAKDataStructure::ResponseOptionInputDiscontainReference);
}
