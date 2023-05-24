/******************************************************************************
 * Copyright 2023 Qsaker(wuuhaii@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part
 * of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in
 * the file LICENCE in the root of the source code directory.
 *****************************************************************************/
#include "SAKCheckBox.hh"
#include "SAKSettings.hh"

SAKCheckBox::SAKCheckBox(QWidget *parent)
    : QCheckBox(parent)
{
    connect(this, &SAKCheckBox::clicked,
            this, &SAKCheckBox::writeToSettingsFile);
}

void SAKCheckBox::setGroupKey(const QString &group,
                              const QString &key)
{
    mKey = group + "/" + key;
    readFromSettingsFile();
}


void SAKCheckBox::readFromSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    bool ret = SAKSettings::instance()->value(mKey).toBool();
    setChecked(ret);
}

void SAKCheckBox::writeToSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    SAKSettings::instance()->setValue(mKey, isChecked());
}
