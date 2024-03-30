/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCheckBox.h"

#include "xToolsSettings.h"

xToolsCheckBox::xToolsCheckBox(QWidget* parent)
    : QCheckBox(parent)
{
    connect(this, &xToolsCheckBox::clicked, this, &xToolsCheckBox::writeToSettingsFile);
}

void xToolsCheckBox::setGroupKey(const QString& group, const QString& key)
{
    m_key = group + "/" + key;
    readFromSettingsFile();
}

void xToolsCheckBox::readFromSettingsFile()
{
    if (m_key.isEmpty()) {
        return;
    }

    bool ret = xToolsSettings::instance()->value(m_key).toBool();
    setChecked(ret);
}

void xToolsCheckBox::writeToSettingsFile()
{
    if (m_key.isEmpty()) {
        return;
    }

    xToolsSettings::instance()->setValue(m_key, isChecked());
}
