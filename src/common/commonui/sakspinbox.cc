/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "sakspinbox.h"

#include "saksettings.h"

SAKSpinBox::SAKSpinBox(QWidget* parent) : QSpinBox(parent) {
  connect(this, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
          this, &SAKSpinBox::writeToSettingsFile);
}

void SAKSpinBox::setGroupKey(const QString& group, const QString& key) {
  mKey = group + "/" + key;
  readFromSettingsFile();
}

void SAKSpinBox::readFromSettingsFile() {
  if (mKey.isEmpty()) {
    return;
  }

  int value = SAKSettings::instance()->value(mKey).toInt();
  setValue(value);
}

void SAKSpinBox::writeToSettingsFile() {
  if (mKey.isEmpty()) {
    return;
  }

  SAKSettings::instance()->setValue(mKey, value());
}
