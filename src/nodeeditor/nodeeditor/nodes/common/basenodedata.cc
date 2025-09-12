/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "basenodedata.h"

BaseNodeData::BaseNodeData(const QByteArray &data)
    : m_bytes(data)
{}

QtNodes::NodeDataType BaseNodeData::type() const
{
    return QtNodes::NodeDataType{"QByteArray", "QByteArray"};
}

QByteArray BaseNodeData::bytes()
{
    return m_bytes;
}
