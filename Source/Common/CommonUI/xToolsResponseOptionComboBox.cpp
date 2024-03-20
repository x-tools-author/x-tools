/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsResponseOptionComboBox.h"

#include "xToolsDataStructure.h"

xToolsResponseOptionComboBox::xToolsResponseOptionComboBox(QWidget* parent)
    : xToolsComboBox(parent)
{
#if 0
    addItem(tr("Disable"), SAKDataStructure::ResponseOptionDisable);
#endif
    addItem(tr("Echo", "widget", __LINE__), xToolsDataStructure::ResponseOptionEcho);
    addItem(tr("Always", "widget", __LINE__), xToolsDataStructure::ResponseOptionAlways);
    addItem(tr("RxEqualReference", "widget", __LINE__),
            xToolsDataStructure::ResponseOptionInputEqualReference);
    addItem(tr("RxContainReference", "widget", __LINE__),
            xToolsDataStructure::ResponseOptionInputContainReference);
    addItem(tr("RxDiscontainReference", "widget", __LINE__),
            xToolsDataStructure::ResponseOptionInputDiscontainReference);
}
