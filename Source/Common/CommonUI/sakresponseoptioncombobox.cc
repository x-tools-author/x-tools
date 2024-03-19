/***************************************************************************************************
 * Copyright 2023 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakresponseoptioncombobox.h"

#include "sakdatastructure.h"

SAKResponseOptionComboBox::SAKResponseOptionComboBox(QWidget* parent)
    : SAKComboBox(parent)
{
#if 0
    addItem(tr("Disable"), SAKDataStructure::ResponseOptionDisable);
#endif
    addItem(tr("Echo", "widget", __LINE__), SAKDataStructure::ResponseOptionEcho);
    addItem(tr("Always", "widget", __LINE__), SAKDataStructure::ResponseOptionAlways);
    addItem(tr("RxEqualReference", "widget", __LINE__),
            SAKDataStructure::ResponseOptionInputEqualReference);
    addItem(tr("RxContainReference", "widget", __LINE__),
            SAKDataStructure::ResponseOptionInputContainReference);
    addItem(tr("RxDiscontainReference", "widget", __LINE__),
            SAKDataStructure::ResponseOptionInputDiscontainReference);
}
