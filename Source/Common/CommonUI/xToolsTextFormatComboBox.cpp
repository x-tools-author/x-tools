/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "saktextformatcombobox.h"

#include "sakdatastructure.h"

SAKTextFormatComboBox::SAKTextFormatComboBox(QWidget* parent)
    : SAKComboBox(parent)
{
    addItem("Bin", SAKDataStructure::TextFormatBin);
    addItem("Oct", SAKDataStructure::TextFormatOct);
    addItem("Dec", SAKDataStructure::TextFormatDec);
    addItem("Hex", SAKDataStructure::TextFormatHex);
    addItem("Ascii", SAKDataStructure::TextFormatAscii);
    addItem("Utf8", SAKDataStructure::TextFormatUtf8);

    blockSignals(true);
    setCurrentIndex(5);
    blockSignals(false);
}
