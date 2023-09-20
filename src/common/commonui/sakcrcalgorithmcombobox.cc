/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakcrcalgorithmcombobox.h"

#include <QMetaEnum>

#include "sakcrcinterface.h"

SAKCrcAlgorithmComboBox::SAKCrcAlgorithmComboBox(QWidget* parent)
    : SAKComboBox(parent) {
  QMetaEnum metaEnum =
      QMetaEnum::fromType<SAKCrcInterface::SAKEnumCrcAlgorithm>();
  for (int i = 0; i < metaEnum.keyCount(); i++) {
    addItem(metaEnum.key(i), metaEnum.value(i));
  }
}
