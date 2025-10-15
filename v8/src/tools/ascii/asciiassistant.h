/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QMap>
#include <QObject>

class AsciiModel;
class AsciiModelFilter;
class AsciiAssistant : public QObject
{
    Q_OBJECT
public:
    Q_INVOKABLE AsciiAssistant(QObject* parent = Q_NULLPTR);
    ~AsciiAssistant();

    Q_INVOKABLE QVariant model();
    Q_INVOKABLE void filter(const QString& text);

private:
    AsciiModel* m_model{nullptr};
    AsciiModelFilter* m_modelFilter{nullptr};
};
