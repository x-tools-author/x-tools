/*******************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 ******************************************************************************/
#include "saklineedit.h"

#include "saksettings.h"

SAKLineEdit::SAKLineEdit(QWidget* parent) : QLineEdit(parent) {
  connect(this, &SAKLineEdit::textChanged, this,
          &SAKLineEdit::writeToSettingsFile);
}

void SAKLineEdit::setGroupKey(const QString& group, const QString& key) {
  mKey = group + "/" + key;
  readFromSettingsFile();
}

void SAKLineEdit::readFromSettingsFile() {
  if (mKey.isEmpty()) {
    return;
  }

  QString txt = SAKSettings::instance()->value(mKey).toString();
  setText(txt);
}

void SAKLineEdit::writeToSettingsFile() {
  if (mKey.isEmpty()) {
    return;
  }

  SAKSettings::instance()->setValue(mKey, text());
}
