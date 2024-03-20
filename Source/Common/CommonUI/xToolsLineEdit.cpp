/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsLineEdit.h"

#include "xToolsSettings.h"

xToolsLineEdit::xToolsLineEdit(QWidget* parent)
    : QLineEdit(parent)
{
    connect(this, &xToolsLineEdit::textChanged, this, &xToolsLineEdit::writeToSettingsFile);
}

void xToolsLineEdit::setGroupKey(const QString& group, const QString& key)
{
    mKey = group + "/" + key;
    readFromSettingsFile();
}

void xToolsLineEdit::readFromSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    QString txt = xToolsSettings::instance()->value(mKey).toString();
    setText(txt);
}

void xToolsLineEdit::writeToSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    xToolsSettings::instance()->setValue(mKey, text());
}
