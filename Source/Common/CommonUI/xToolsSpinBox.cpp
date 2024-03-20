/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsSpinBox.h"

#include "xToolsSettings.h"

xToolsSpinBox::xToolsSpinBox(QWidget* parent)
    : QSpinBox(parent)
{
    connect(this,
            static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged),
            this,
            &xToolsSpinBox::writeToSettingsFile);
}

void xToolsSpinBox::setGroupKey(const QString& group, const QString& key)
{
    mKey = group + "/" + key;
    readFromSettingsFile();
}

void xToolsSpinBox::readFromSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    int value = xToolsSettings::instance()->value(mKey).toInt();
    setValue(value);
}

void xToolsSpinBox::writeToSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    xToolsSettings::instance()->setValue(mKey, value());
}
