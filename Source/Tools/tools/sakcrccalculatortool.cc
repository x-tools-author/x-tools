/***************************************************************************************************
 * Copyright 2023 Qsaker(qsaker@foxmail.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of QtSwissArmyKnife project.
 *
 * QtSwissArmyKnife is licensed according to the terms in the file LICENCE in the root of the source
 * code directory.
 **************************************************************************************************/
#include "sakcrccalculatortool.h"

#include "sakcrcinterface.h"

SAKCrcCalculatorTool::SAKCrcCalculatorTool(QObject *parent)
    : SAKBaseTool(parent)
{
    m_startIndex = 0;
    m_endIndex = 0;
    m_algorithm = SAKCrcInterface::CRC_8;
    m_bigEndian = false;
    m_crcInterface = new SAKCrcInterface(this);
}

SAKCrcCalculatorTool::~SAKCrcCalculatorTool() {}

int SAKCrcCalculatorTool::startIndex() const
{
    return m_startIndex;
}

void SAKCrcCalculatorTool::setStartIndex(int index)
{
    m_startIndex = index;
}

int SAKCrcCalculatorTool::endIndex() const
{
    return m_endIndex;
}

void SAKCrcCalculatorTool::setEndIndex(int index)
{
    m_endIndex = index;
}

int SAKCrcCalculatorTool::algorithm() const
{
    return m_algorithm;
}

void SAKCrcCalculatorTool::setAlgorithm(int algorithm)
{
    m_algorithm = algorithm;
}

bool SAKCrcCalculatorTool::bigEndian() const
{
    return m_bigEndian;
}

void SAKCrcCalculatorTool::setBigEndian(bool bigEndian)
{
    m_bigEndian = bigEndian;
}

void SAKCrcCalculatorTool::inputBytes(const QByteArray &bytes)
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
