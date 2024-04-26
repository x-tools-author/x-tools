/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsComboBox.h"

#include <QLineEdit>

#include "xToolsSettings.h"

xToolsComboBox::xToolsComboBox(QWidget* parent)
    : QComboBox(parent)
{
    connect(this, &xToolsComboBox::currentTextChanged, this, &xToolsComboBox::writeToSettingsFile);
    setSizeAdjustPolicy(QComboBox::AdjustToContents);
}

void xToolsComboBox::setCurrentIndexFromData(const QVariant& data)
{
    int ret = findData(data);
    if (ret != -1) {
        setCurrentIndex(ret);
    }
}

void xToolsComboBox::setGroupKey(const QString& group, const QString& key, bool isIndex)
{
    mKey = group + "/" + key;
    mIsIndex = isIndex;

    readFromSettingsFile();
}

void xToolsComboBox::readFromSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    QVariant var = xToolsSettings::instance()->value(mKey);
    if (!var.isValid()) {
        return;
    }

    if (mIsIndex) {
        int index = var.toInt();
        if (index >= 0 && index < count()) {
            setCurrentIndex(index);
        }
        return;
    }

    int index = findData(var);
    if (index >= 0 && index < count()) {
        setCurrentIndex(index);
        return;
    }

    if (isEditable()) {
        setCurrentText(var.toString());
    }
}

void xToolsComboBox::writeToSettingsFile()
{
    if (mKey.isEmpty()) {
        return;
    }

    if (mIsIndex) {
        xToolsSettings::instance()->setValue(mKey, currentIndex());
    } else {
        if (isEditable()) {
            xToolsSettings::instance()->setValue(mKey, currentText());
        } else {
            xToolsSettings::instance()->setValue(mKey, currentData());
        }
    }
}
