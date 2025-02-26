/***************************************************************************************************
 * Copyright 2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <private/qobject_p.h>

#include <QSettings>

class ApplicationPrivate : QObjectPrivate
{
    Q_DECLARE_PUBLIC(Application);

public:
    ApplicationPrivate() {}

public:
    QSettings *m_settings = nullptr;
}