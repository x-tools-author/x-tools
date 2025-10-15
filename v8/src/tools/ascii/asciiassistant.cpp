/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "asciiassistant.h"

#include "asciimodel.h"
#include "asciimodelfilter.h"

AsciiAssistant::AsciiAssistant(QObject* parent)
    : QObject(parent)
{
    m_model = new AsciiModel(this);
    m_modelFilter = new AsciiModelFilter(this);
    m_modelFilter->setSourceModel(m_model);
}

AsciiAssistant::~AsciiAssistant() {}

QVariant AsciiAssistant::model()
{
    return QVariant::fromValue(m_modelFilter);
}

void AsciiAssistant::filter(const QString& text)
{
    m_modelFilter->setFilterFixedString(text);
    m_modelFilter->invalidate();
}
