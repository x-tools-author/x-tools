/***************************************************************************************************
 * Copyright 2026-2026 x-tools-author(x-tools@outlook.com). All rights reserved.
 *
 * The file is encoded using "utf8 with bom", it is a part of xModbus project.
 *
 * xTools is licensed according to the terms in the file LICENCE(GPL V3) in the root of the source
 * code directory.
 **************************************************************************************************/
#pragma once

#include <QComboBox>
#include <QObject>
#include <QSpinBox>

namespace xCoAP {

class CoAPCommon : public QObject
{
    Q_OBJECT
public:
    explicit CoAPCommon(QObject* parent = nullptr);
    ~CoAPCommon();

    static void setupSocketAddress(QComboBox* comboBox);
    static void setupSocketPort(QSpinBox* comboBox);
    static void setupSocketProtocol(QComboBox* comboBox);
};

} // namespace xCoAP
