/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#include "xToolsCrcCalculatorTool.h"

#include "xToolsCrcInterface.h"

xToolsCrcCalculatorTool::xToolsCrcCalculatorTool(QObject *parent)
    : xToolsBaseTool(parent)
{
    m_startIndex = 0;
    m_endIndex = 0;
    m_algorithm = xToolsCrcInterface::CRC_8;
    m_bigEndian = false;
    m_crcInterface = new xToolsCrcInterface(this);
}

xToolsCrcCalculatorTool::~xToolsCrcCalculatorTool() {}

int xToolsCrcCalculatorTool::startIndex() const
{
    return m_startIndex;
}

void xToolsCrcCalculatorTool::setStartIndex(int index)
{
    m_startIndex = index;
}

int xToolsCrcCalculatorTool::endIndex() const
{
    return m_endIndex;
}

void xToolsCrcCalculatorTool::setEndIndex(int index)
{
    m_endIndex = index;
}

int xToolsCrcCalculatorTool::algorithm() const
{
    return m_algorithm;
}

void xToolsCrcCalculatorTool::setAlgorithm(int algorithm)
{
    m_algorithm = algorithm;
}

bool xToolsCrcCalculatorTool::bigEndian() const
{
    return m_bigEndian;
}

void xToolsCrcCalculatorTool::setBigEndian(bool bigEndian)
{
    m_bigEndian = bigEndian;
}

void xToolsCrcCalculatorTool::inputBytes(const QByteArray &bytes)
{
    QByteArray crc = m_crcInterface->calculateBytes(bytes,
                                                    m_algorithm,
                                                    m_startIndex,
                                                    m_endIndex,
                                                    m_bigEndian);
    QByteArray cookedBytes = bytes;
    cookedBytes.append(crc);
    
    emit outputBytes(cookedBytes);
}
