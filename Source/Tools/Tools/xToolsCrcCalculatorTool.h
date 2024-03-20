/***************************************************************************************************
 * Copyright 2023-2024 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xToolsBaseTool.h"

class xToolsCrcInterface;
class xToolsCrcCalculatorTool : public xToolsBaseTool
{
    Q_OBJECT
public:
    xToolsCrcCalculatorTool(QObject *parent = Q_NULLPTR);
    ~xToolsCrcCalculatorTool();

    int startIndex() const;
    void setStartIndex(int index);
    int endIndex() const;
    void setEndIndex(int index);
    int algorithm() const;
    void setAlgorithm(int algorithm);
    bool bigEndian() const;
    void setBigEndian(bool bigEndian);

    void inputBytes(const QByteArray &bytes) override;

private:
    int m_startIndex;
    int m_endIndex;
    int m_algorithm;
    bool m_bigEndian;
    xToolsCrcInterface *m_crcInterface;
};
