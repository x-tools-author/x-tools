/***************************************************************************************************
 * Copyright 2020-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QObject>
#include <QVariant>

#if QT_VERSION >= QT_VERSION_CHECK(6, 0, 0)
#define xSkipEmptyParts Qt::SkipEmptyParts
#else
#define xSkipEmptyParts QString::SkipEmptyParts
#endif

namespace xTools {

class xTools : public QObject
{
    Q_OBJECT
public:
    explicit xTools(QObject* parent = nullptr);
    ~xTools() override;

    static QByteArray byteArray2Hex(const QByteArray& source, char separator = '\0');
};

} // namespace xTools
