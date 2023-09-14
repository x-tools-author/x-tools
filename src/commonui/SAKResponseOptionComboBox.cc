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
#include "SAKResponseOptionComboBox.h"

SAKResponseOptionComboBox::SAKResponseOptionComboBox(QWidget *parent)
    : SAKComboBox(parent)
{
#if 0
    addItem(tr("Disable"), SAKDataStructure::ResponseOptionDisable);
#endif
    addItem(tr("Echo", "widget", __LINE__),
            SAKDataStructure::ResponseOptionEcho);
    addItem(tr("Always", "widget", __LINE__),
            SAKDataStructure::ResponseOptionAlways);
    addItem(tr("RxEqualReference", "widget", __LINE__),
            SAKDataStructure::ResponseOptionInputEqualReference);
    addItem(tr("RxContainReference", "widget", __LINE__),
            SAKDataStructure::ResponseOptionInputContainReference);
    addItem(tr("RxDiscontainReference", "widget", __LINE__),
            SAKDataStructure::ResponseOptionInputDiscontainReference);
}
