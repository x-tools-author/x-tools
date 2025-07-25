/***************************************************************************************************
 * Copyright 2025-2025 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xTools project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include "xui.h"

class BarCodeAssistant;
class xCode : public xUi
{
    Q_OBJECT
public:
    explicit xCode(QWidget *parent = nullptr);
    ~xCode() override;

    void generateBarcode();

private:
    BarCodeAssistant *m_barCodeAssistant{nullptr};
};