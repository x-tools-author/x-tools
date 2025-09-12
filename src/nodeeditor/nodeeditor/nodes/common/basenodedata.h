/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xFlow project.
 *
 * xFlow is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QtNodes/NodeData>

class BaseNodeData : public QtNodes::NodeData
{
public:
    BaseNodeData(const QByteArray &data = QByteArray());
    virtual QtNodes::NodeDataType type() const override;

    QByteArray bytes();

private:
    QByteArray m_bytes;
};
