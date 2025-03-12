/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "application.h"

class xDebug : public Application
{
    Q_OBJECT
public:
    explicit xDebug(int argc, char **argv);
    ~xDebug() override;

    Q_INVOKABLE QString version();
    Q_INVOKABLE QString bytes2string(const QByteArray &bytes, int format);
    Q_INVOKABLE QByteArray string2bytes(const QString &txt, int format);
    Q_INVOKABLE QByteArray arrayAppendArray(const QByteArray &array1, const QByteArray &array2);
    Q_INVOKABLE QString cookedEscapeCharacter(const QString &txt, int esc);
    Q_INVOKABLE QByteArray cookedAffixes(int affixes);
    Q_INVOKABLE void setQuickTextDocumentMaximumBlockCount(QVariant textDocument, int count);
    Q_INVOKABLE QString dateTimeString(const QString &format);
};
