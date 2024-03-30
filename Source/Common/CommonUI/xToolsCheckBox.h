/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QCheckBox>

class xToolsCheckBox : public QCheckBox
{
    Q_OBJECT
public:
    xToolsCheckBox(QWidget *parent = nullptr);
    void setGroupKey(const QString &group, const QString &key);

private:
    QString m_key;

private:
    void readFromSettingsFile();
    void writeToSettingsFile();
};
